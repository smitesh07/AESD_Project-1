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
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <mqueue.h>
#include "log.h"
#include "timer.h"
#include "tempSensor.h"
#include "queue.h"
#include "lumSensor.h"
#include "socket.h"
#include "i2c.h"

//Semaphore to synchronize access the I2C bus
#define SEM_I2C "/sem_i2c"

#define HEARTBEAT_TIMEOUT 5 //in seconds


/**
 * @brief Timer callback function to check the heartbeat messages from individual threads
 * 
 */
void heartbeatTimerHandler () {
    //Check the message queue for heartbeat messages from individual threads
    printf("\nMain thread heartbeat timeout\n");
    fflush(stdout);
    return;
}

pthread_t logger, tempSensor, lumSensor, externSocket;

/**
 * @brief Main
 * 
 */
int main(int argc, char *argv[])
{
    char logFile[50];
    char fileName[50];

    if (argc > 1) {
      strcpy(logFile, argv[1]);
      strcpy(fileName, argv[2]);
      strcat(logFile,fileName);
    } else {
        strcpy(logFile, "log.txt");
    }

    char path[] = "/OpenTest_MQ";
    logInit(logFile);
    initQueue(path);

    printf("\nMain spawned");
    fflush(stdout);

    //Create and Initialize the semaphore for I2C bus syncronization
    sem_i2c = sem_open(SEM_I2C, O_RDWR | O_CREAT, 0666, 1);
    if (sem_i2c == SEM_FAILED || sem_i2c == SEM_FAILED)
        perror("sem_open failed\n");

    i2cOpen();

    pthread_create (&logger, NULL, loggerHandler, NULL);
    pthread_create (&tempSensor, NULL, tempSensorHandler, NULL);
    pthread_create (&lumSensor, NULL, lumSensorHandler, NULL);  
    pthread_create (&externSocket, NULL, externSocketHandler, NULL);

    initTimer(HEARTBEAT_TIMEOUT *1000000000, heartbeatTimerHandler);


    pthread_join(logger, NULL);
	pthread_join(tempSensor, NULL);
    pthread_join(lumSensor, NULL);
    pthread_join(externSocket, NULL);
    logFlush();
    mq_unlink (path);

    return 0;
}