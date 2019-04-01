/**
 * @file main.c
 * @author Smitesh Modak and Ashish Tak
 * @brief
 * @version 0.1
 * @date 2019-03-18
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <mqueue.h>
#include <signal.h>
#include "log.h"
#include "timer.h"
#include "tempSensor.h"
#include "queue.h"
#include "lumSensor.h"
#include "socket.h"
#include "i2c.h"
#include "SimpleGPIO.h"


//Semaphore to synchronize access the I2C bus
#define SEM_I2C "/sem_i2c"

#define HEARTBEAT_TIMEOUT 10 //in seconds
#define FILENAME_MAX_LEN 50

pthread_t logger, tempSensor, lumSensor, externSocket;

//Global flag to be set by main on receiving the SIGINT signal
bool terminateSignal= false;

timer_t mainTimerid;


/**
 * @brief Timer callback function to check (and reset) the heartbeat flags from individual threads
 *
 */
void heartbeatTimerHandler () {

    if (tempHeartbeatFlag)
        tempHeartbeatFlag=false;
    else {
        enQueueForLog(ERROR, "Temperature sensing thread is DEAD!! Issuing pthread_cancel().. ", 0);
        pthread_cancel(&tempSensor);
    }

    if (lumHeartbeatFlag)
        lumHeartbeatFlag=false;
    else {
        enQueueForLog(ERROR, "Luminosity sensing thread is DEAD!! Issuing pthread_cancel().. ", 0);
        pthread_cancel(&lumSensor);
    }

    if (logHeartbeatFlag)
        logHeartbeatFlag=false;
    else {
        enQueueForLog(ERROR, "Logger thread is DEAD!! Issuing pthread_cancel().. ", 0);
        deQueueFromLog();
        fflush(filePtr);
        pthread_cancel(&logger);
    }

    return;
}


/**
 * @brief Signal handler for the SIGINT signal
 *
 * @param signal
 */
void sigHandler (int signal) {
	switch (signal) {
		case SIGINT:
            //Set the flag to give a termination signal to all threads.
            //Since the flag is written to only once in the lifetime of
            //the program, no synchronization is required
			terminateSignal=true;
            //Explictly send a signal to the External Socket handling thread,
            //since its blocking on external requests and does not periodically
            //monitor any termination signal / flag
            pthread_kill(externSocket, SIGUSR1);
			break;
		default:
			break;
	}
}


/**
 * @brief Main
 *
 */
int main(int argc, char *argv[])
{
    char logFile[FILENAME_MAX_LEN];
    char fileName[FILENAME_MAX_LEN];

    if (argc > 1) {
      strcpy(logFile, argv[1]);
      strcpy(fileName, argv[2]);
      strcat(logFile,fileName);
    } 
    else {
        strcpy(logFile, "log.txt");
    }

    char path[] = "/OpenTest_MQ";
    logInit(logFile);
    initQueue(path);

    //Create and Initialize the semaphore for I2C bus syncronization
    sem_i2c = sem_open(SEM_I2C, O_RDWR | O_CREAT, 0666, 1);
    if (sem_i2c == SEM_FAILED || sem_i2c == SEM_FAILED)
        perror("sem_open failed\n");

    i2cOpen();

    //Register the signal handler for the termination signal (SIGINT) from the user
    struct sigaction sa;
	sigemptyset (&sa.sa_mask);
	sa.sa_handler=&sigHandler;
	sa.sa_flags=0;
	sigaction(SIGINT, &sa, NULL);

    gpio_export(ERROR_LED);
    gpio_set_dir(ERROR_LED, OUTPUT_PIN);

    pthread_create (&logger, NULL, loggerHandler, NULL);
    pthread_create (&tempSensor, NULL, tempSensorHandler, NULL);
    pthread_create (&lumSensor, NULL, lumSensorHandler, NULL);
    pthread_create (&externSocket, NULL, externSocketHandler, NULL);

    mainTimerid= initTimer(HEARTBEAT_TIMEOUT*(uint64_t)1000000000, heartbeatTimerHandler);

    pthread_join(tempSensor, NULL);
    pthread_join(lumSensor, NULL);
    pthread_join(externSocket, NULL);
    pthread_join(logger, NULL);

    enQueueForLog(WARN, "Main thread terminating..", 0);
    deQueueFromLog();
    fflush(filePtr);
    logFlush();

    //Cleanup procedure for main()
    timer_delete(mainTimerid);
    i2cClose();
    sem_unlink(SEM_I2C);
    mq_close(mqdes);
    mq_unlink (path);

    return 0;
}
