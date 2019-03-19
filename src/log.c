/*
 * log.c
 *
 *  Created on: March 17, 2019
 *  Author: Smitesh Modak and Ashish Tak
 */

#include "log.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <sys/time.h>

FILE *filePtr;

/**
 * @return a timestamp value for the logger
 * This will be printed at the beginning of each log message.
 */
uint32_t loggerGetTimestamp(void) {
  /* Current date and time */
  struct timeval cur_tv;
  struct timezone *cur_tz = NULL;
  gettimeofday(&cur_tv,cur_tz);
  uint32_t timeStamp = (cur_tv.tv_sec/1000000)+cur_tv.tv_usec;

  return timeStamp;
}

/**
 * Initialize logging
 */
void logInit(char *logFile) {
    char fileName[10];
    strcpy(fileName, logFile);
    filePtr = fopen((char *)fileName,"w");
    LOG_QUEUE queue;
    queue.level = ERROR;
    queue.msg = "ERROR test";
    deQueueFromLog(&queue);
    queue.level = INFO;
    queue.msg = "ERROR test";
    queue.value = 11;
    deQueueFromLog(&queue);
    queue.level = WARN;
    queue.msg = "ERROR test";
    deQueueFromLog(&queue);

}


/**
 * Flush log file
 */
void logFlush(void) { 
  fflush(filePtr);
  fclose(filePtr);
}


void loggerHeartbeatTimerHandler () {
  //Send a heartbeat message onto the message queue
  printf("\nLogging thread heartbeat timeout");
  return;
}


void loggerHandler() {
    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    initTimer(threadID, 2000000000, loggerHeartbeatTimerHandler);
    while (1) {
      sleep(1);
    }
}

void enQueueForLog(LOG_QUEUE *logQueue) {

}

/**
 * @brief dequeue from the logger queue and LOG it
 * 
 * @param logQueue 
 */

void deQueueFromLog(LOG_QUEUE *logQueue) {
  if (logQueue->level == ERROR)
    {
      LOG_ERROR("Error writing successful");
    }
  else if (logQueue->level == INFO)
    {
      LOG_INFO("Info writing successful %d",logQueue->value);
    }
  else if (logQueue->level == WARN)
    {
      LOG_WARN("Warn writing successful");
    }
}