/*
 * log.c
 *
 *  Created on: March 17, 2019
 *  Author: Smitesh Modak and Ashish Tak
 *  Reference: Dan Walkes, ECEN 5813-IoT Embedded Firmware (CU Boulder)
 */

#include "log.h"
#include <stdbool.h>
#include "time.h"

#if INCLUDE_LOGGING
/**
 * @return a timestamp value for the logger, typically based on a free running
 * timer.
 * This will be printed at the beginning of each log message.
 */
uint32_t loggerGetTimestamp(void) {
  /* Current date and time */
	struct timeval cur_tv;
  struct timezone *cur_tz = NULL;
  gettimeofday(&cur_tv,cur_tz);
  uint32_t time_stamp = (cur_tv.tv_sec/1000000)+cur_tv.tv_usec;

  return time_stamp;
}

/**
 * Initialize logging
 */
void logInit(void) {
  LOG_INFO(file_ptr,"Initialized Logging");
}

/**
 * Initialize logging
 * 
 */
void logFlush(void) { 
  char *file_name = "test_log.txt";
  FILE *file_ptr = fopen((char *)file_name,"a");
  fflush(file_ptr);
  fclose(file_ptr);
}
#endif
