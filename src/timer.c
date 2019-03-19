/**
 * @file timer.c
 * @author Ashish Tak, Smitesh Modak
 * @brief 
 * @version 0.1
 * @date 2019-03-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "timer.h"

/**
 * @brief Timer Initialization helper function used by all the threads
 * 
 * @param threadID - The thread to which the timeout signal needs to be sent 
 * @param nanosec - Timeout interval in nanoseconds (used for both the initial timeout and repetitive interval)
 * @param *callbackFunction() - The callback function which needs to be associated with the timer
 */
void initTimer(pid_t threadID, uint64_t nanosec, void (*callbackFunction()) ) {
    timer_t timerid;
	struct itimerspec its;
	struct sigevent sev;
	struct sigaction tsa;

	tsa.sa_flags = 0;
	sigemptyset (&tsa.sa_mask);
	tsa.sa_handler=&callbackFunction;
	sigaction(SIGRTMIN, &tsa, NULL);

	sev.sigev_notify = SIGEV_THREAD_ID;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;
    sev.sigev_notify_thread_id = threadID;

	timer_create(CLOCK_REALTIME, &sev, &timerid);
	
	its.it_value.tv_sec=nanosec/1000000000;
	its.it_value.tv_nsec=nanosec%1000000000;
	its.it_interval.tv_sec =nanosec/1000000000;
	its.it_interval.tv_nsec = nanosec%1000000000;

	timer_settime(timerid, 0, &its, NULL);
}