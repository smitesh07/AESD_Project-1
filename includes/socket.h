/**
 * @file socket.h
 * @author Smitesh Modak and Ashish Tak
 * @brief : Header file for the Remote Socket Request Handler
 * @version 0.1
 * @date 2019-03-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include "lumSensor.h"
#include "tempSensor.h"


//Prototypes

/**
 * @brief Handler function / Entry point for the External Socket handling thread
 * 
 */
void *externSocketHandler (void *arg);