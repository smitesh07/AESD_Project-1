#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
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