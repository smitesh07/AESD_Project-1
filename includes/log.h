/**
 * @file log.h
 * @author Smitesh Modak and Ashish Tak
 * @brief log header file containing enums, structures and function prototypes
 * @version 0.1
 * @date 2019-03-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SRC_LOG_H_
#define SRC_LOG_H_
#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include "timer.h"

extern FILE *filePtr;

/*
* ERROR for unexpected conditions which should never occur
* WARN for potential issues which may occur
* INFO for infrequent status updates
* DEBUG for detailed updates, useful when troubleshooting a specific code path
*/

typedef enum {
  ERROR,
  WARN,
  INFO,
  DEBUG
} LOG_LEVEL;

/**
*  log structure for queue
*/
typedef struct {
  LOG_LEVEL level;
  char msg[50];
  float value;
  // pid_t pid;
} LOG_t;

#ifndef LOG_ERROR
#define LOG_ERROR(message, ...) LOG_DO(filePtr, message, ERROR, ##__VA_ARGS__)
#endif

#ifndef LOG_WARN
#define LOG_WARN(message, ...) LOG_DO(filePtr, message, WARN, ##__VA_ARGS__)
#endif

#ifndef LOG_INFO
#define LOG_INFO(message, ...) LOG_DO(filePtr, message, INFO, ##__VA_ARGS__)
#endif

#if INCLUDE_LOG_DEBUG
#ifndef LOG_DEBUG
#define LOG_DEBUG(message, ...) LOG_DO(filePtr, message, DEBUG, ##__VA_ARGS__)
#define LOG_DEBUG_CODE(code) code
#endif
#else
#define LOG_DEBUG(message, ...)
#define LOG_DEBUG_CODE(code)
#endif

//#if INCLUDE_LOGGING
#define LOG_DO(log_file_ptr, message, level, ...)                              \
  fprintf(filePtr,"%5" PRIu32 ":%d:%s: " message "\n", loggerGetTimestamp(), level, \
         __func__, ##__VA_ARGS__)


//Prototypes


/**
 * @brief Handler function / Entry point for the logging thread
 * 
 */
void *loggerHandler(void *arg);

/**
 * @brief Initialize logging
 * 
 */
void logInit();

/**
 * @brief This will be printed at the beginning of each log message.
 * 
 * @return uint32_t 
 *         return a timestamp value for the logger
 */
uint32_t loggerGetTimestamp();

/**
 * @brief flush to log file
 * 
 */
void logFlush();

#endif /* SRC_LOG_H_ */
