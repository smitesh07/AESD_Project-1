#ifndef SRC_TEMPSENSOR_H_
#define SRC_TEMPSENSOR_H_

#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "timer.h"

extern float tempData;

typedef struct {
    bool sensorConnected;
    float temp;
}tempUpdate;

//Prototypes

/**
 * @brief Handler function / Entry point for the Temperature Sensing thread
 * 
 */
void *tempSensorHandler (void *arg);


/**
 * @brief Timer ISR to send a heartbeat message from the Temperature Sensing thread onto the message queue
 * 
 */
// void tempHeartbeatTimerHandler (void);


/**
 * @brief Timer ISR to trigger reading a temperature value from the sensor
 * 
 */
void tempSensorTrigger (void);

#endif /* SRC_TEMPSENSOR_H_ */