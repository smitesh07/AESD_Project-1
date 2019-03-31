#ifndef LUMSENSOR_H_
#define LUMSENSOR_H_

#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "timer.h"
#include "queue.h"
#include "log.h"
#include "apds9301.h"


typedef enum {
    DARK=0,
    LIGHT
}lumState;

typedef struct {
    bool sensorConnected;
    float lux;
    lumState brightnessState;  
}luxUpdate;

//Prototypes

/**
 * @brief Handler function / Entry point for the Luminosity Sensing thread
 * 
 */
void *lumSensorHandler (void *arg);

/**
 * @brief Timer ISR to trigger reading a luminosity value from the sensor
 * 
 */
void lumSensorTrigger (void);

luxUpdate * externReadLum(void);

#endif