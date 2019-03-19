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
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "log.h"
#include "timer.h"
#include "tempSensor.h"


/**
 * @brief Timer callback function to check the heartbeat messages from the Message Queue
 * 
 */
void heartbeatTimerHandler () {
    //Check the message queue for heartbeat messages from individual threads
    printf("\nMain thread heartbeat timeout");
    return;
}


/**
 * @brief Initialize the timer responsible for checking heartbeat messages from all other threads
 * 
 */
/*
static void initHeartbeatTimer(void) {
    timer_t timerid;
	struct itimerspec its;
	struct sigevent sev;
	struct sigaction tsa;

	tsa.sa_flags = 0;
	sigemptyset (&tsa.sa_mask);
	tsa.sa_handler=&heartbeatTimerHandler;
	sigaction(SIGRTMIN, &tsa, NULL);

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;

	timer_create(CLOCK_REALTIME, &sev, &timerid);
	
	its.it_value.tv_sec=0;
	its.it_value.tv_nsec=100000000;
	its.it_interval.tv_sec =0;
	its.it_interval.tv_nsec = 100000000;

	timer_settime(timerid, 0, &its, NULL);
}
*/


/**
 * @brief Main
 * 
 */
void main()
{
    char *file_name = "test";
    FILE *file_ptr = fopen((char *)file_name,"w");
    LOG_INFO(file_ptr, "HELLO");
    fclose(file_ptr);


    pthread_t logger, tempSensor, lumSensor, externSocket;

    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    initTimer(threadID, 2000000000, heartbeatTimerHandler);

    pthread_create (&logger, NULL, &loggerHandler, NULL);
    pthread_create (&tempSensor, NULL, &tempSensorHandler, NULL);
    /*
    pthread_create (&lumSensor, NULL, &lumSensorHandler, NULL);  
    pthread_create (&externSocket, NULL, &externSocketHandler, NULL);
    */




    pthread_join(logger, NULL);
	pthread_join(tempSensor, NULL);
    /*
    pthread_join(lumSensor, NULL);
    pthread_join(externSocket, NULL);
    */

}
