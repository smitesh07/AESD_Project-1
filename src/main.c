/**
 * @file main.c
 * @author Ashish Tak, Smitesh Modak
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
#include "log.h"
#include "timer.h"
#include "tempSensor.h"
#include "queue.h"
#include "lumSensor.h"

int msgid;


/**
 * @brief Timer callback function to check the heartbeat messages from the Message Queue
 * 
 */
void heartbeatTimerHandler () {
    //Check the message queue for heartbeat messages from individual threads
    printf("\nMain thread heartbeat timeout");
    fflush(stdout);
    return;
}


/**
 * @brief Main
 * 
 */
void main()
{
    pthread_t logger, tempSensor, lumSensor, externSocket;

    char *logFile = "log.txt"; 
    const char *path = "Text";
    logInit(logFile);
    initQueue(msgid,path);
    enQueueForLog(msgid,1, "Lets do this", 2);
    deQueueFromLog(msgid);
    logFlush();

    printf("\nMain spawned");
    fflush(stdout);

    pthread_create (&logger, NULL, loggerHandler, NULL);
    pthread_create (&tempSensor, NULL, tempSensorHandler, NULL);
    pthread_create (&lumSensor, NULL, lumSensorHandler, NULL);  

    /*
    pthread_create (&externSocket, NULL, externSocketHandler, NULL);
    */

    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    initTimer(threadID, 2*1000000000, heartbeatTimerHandler);


    pthread_join(logger, NULL);
	pthread_join(tempSensor, NULL);
    pthread_join(lumSensor, NULL);

    /*
    pthread_join(externSocket, NULL);
    */


}