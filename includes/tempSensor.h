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


tempUpdate * externReadTemp(void);

#endif /* SRC_TEMPSENSOR_H_ */