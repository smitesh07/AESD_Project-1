#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "timer.h"

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