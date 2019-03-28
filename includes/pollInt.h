/**
 * @file pollInt.h
 * @author Smitesh Modak and Ashish Tak
 * @brief header file with structure definition and function prototypes
 * @version 0.1
 * @date 2019-03-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SRC_POLLINT_H_
#define SRC_POLLINT_H_

#include <poll.h>

typedef struct {
    struct pollfd poll_fds[1];      // structure describing a polling request
    int f;                          // file handler
    char value[4];                  // used to store gpio value
}poll_t;

/**
 * @brief intialize polling mechanism
 * 
 * @param gpio      GPIO interrupt pin
 * @param pollFds   address of structure describing a polling request
 */
void pollInit(unsigned int gpio, poll_t *pollFds);

#endif /* SRC_POLLINT_H_ */