/**
 * @file log.c
 * @author Smitesh Modak and Ashish Tak
 * @brief log source file that initializes log file and writes out to log file
 * @version 0.1
 * @date 2019-03-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <sys/time.h>
#include "log.h"
#include "queue.h"

FILE *filePtr;

uint32_t loggerGetTimestamp(void) {
  /* Current date and time */
  struct timeval cur_tv;
  struct timezone *cur_tz = NULL;
  gettimeofday(&cur_tv,cur_tz);
  uint32_t timeStamp = (cur_tv.tv_sec/1000000)+cur_tv.tv_usec;

  return timeStamp;
}

void logInit(char *logFile) {
    char fileName[10];
    strcpy(fileName, logFile);
    filePtr = fopen((char *)fileName,"w");
    // LOG_t queue;
    // queue.level = ERROR;
    // queue.msg = "ERROR test";
    // deQueueFromLog(&queue);
    // queue.level = INFO;
    // queue.msg = "ERROR test";
    // queue.value = 11;
    // deQueueFromLog(&queue);
    // queue.level = WARN;
    // queue.msg = "ERROR test";
    // deQueueFromLog(&queue);

}

void logFlush(void) { 
  fflush(filePtr);
  fclose(filePtr);
}

/*
void loggerHeartbeatTimerHandler () {
  //Send a heartbeat message onto the message queue
  printf("\nLogging thread heartbeat timeout");
  fflush(stdout);
  return;
}
*/


void *loggerHandler(void *arg) {
    printf("\nLogger thread spawned");
    fflush(stdout);
    // uint32_t threadID= (pid_t)syscall(SYS_gettid);
    // initTimer(threadID, 2000000000, loggerHeartbeatTimerHandler);
    while (1) {
      //Call the function HERE to send the heartbeat signal to the message queue
      sleep(1);
    }
}
