/**
 * @file timer.h
 * @author Smitesh Modak and Ashish Tak
 * @brief : Header file for the timer helper functionality
 * @version 0.1
 * @date 2019-03-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


/**
 * @brief Timer Initialization helper function used by all the threads
 * 
 * @param nanosec - Timeout interval in nanoseconds (used for both the initial timeout and repetitive interval)
 * @param *callbackFunction() - The callback function which needs to be associated with the timer
 */
int initTimer(uint64_t nanosec, void (*callbackFunction)());