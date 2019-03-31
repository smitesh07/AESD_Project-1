/**
 * @file tempSensor.h
 * @author Smitesh Modak and Ashish Tak
 * @brief : Header file for the temperature sensing thread operations
 * @version 0.1
 * @date 2019-03-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

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

#define UNIT CELSIUS
extern float tempData;

typedef enum {
    CELSIUS,
    FAHRENHEIT,
    KELVIN
}UNIT_t;
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
 * @brief Timer ISR to trigger reading a temperature value from the sensor
 * 
 */
void tempSensorTrigger (void);

/**
 * @brief API for external tasks to request present Temperature value
 * 
 * @return tempUpdate* : Pointer to the structure containing the Luminosity data
 */
tempUpdate * externReadTemp(void);

#endif /* SRC_TEMPSENSOR_H_ */