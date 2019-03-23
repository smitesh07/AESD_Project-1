/**
 * @file queue.h
 * @author Smitesh Modak and Ashish Tak
 * @brief queue header file containing structures and function prototypes
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include "log.h"

/**
 * @brief message queue structure
 * 
 */
typedef struct {
  long mtype;
  LOG_t logQueue;
}QUEUE_t;

/**
 * @brief 
 * 
 * @param queueName   message queue name
 */
void initQueue(char *queueName);

/**
 * @brief enqueue to the logger queue
 * 
 * @param level   level/priority of message
 * @param msg     message
 * @param value   value from the sensors
 */
void enQueueForLog(LOG_LEVEL level, char *msg, int value);

/**
 * @brief dequeue from the logger queue and LOG it
 * 
 */
void deQueueFromLog(void);

#endif