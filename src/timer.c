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

int initTimer(uint64_t nanosec, void (*callbackFunction)() ) {
    timer_t timerid;
	struct itimerspec its;
	struct sigevent sev;
	struct sigaction tsa;

	//Keep a static count of the number of software timers created
	static uint8_t signalCount=0;

	tsa.sa_flags = 0;
	sigemptyset (&tsa.sa_mask);
	tsa.sa_handler=callbackFunction;
	
	//Set the signal action for the corresponding Real-Time signal, provided it does not exceed the limit
	if (SIGRTMIN + signalCount <= SIGRTMAX) {
		sigaction(SIGRTMIN + signalCount, &tsa, NULL);
	}
	else {
		perror("\nUnable to create more timers");
		return -1;
	}

	sev.sigev_notify = SIGEV_SIGNAL;
	// sev.sigev_notify = SIGEV_THREAD_ID;
	sev.sigev_signo = SIGRTMIN + signalCount;
	// sev.sigev_value.sival_ptr = &timerid;
    // sev.sigev_notify_thread_id = threadID;

	if(timer_create(CLOCK_REALTIME, &sev, &timerid)) {
		perror("\ntimer_create");
		fflush(stdout);
		return -1;
	}
	
	its.it_value.tv_sec=nanosec/1000000000;
	its.it_value.tv_nsec=nanosec%1000000000;
	its.it_interval.tv_sec =nanosec/1000000000;
	its.it_interval.tv_nsec = nanosec%1000000000;
	
	if(timer_settime(timerid, 0, &its, NULL)) {
		perror("timer_settime");
		fflush(stdout);
		return -1;
	}

	++signalCount;

	return 0;
}