#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include "timer.h"

//Prototypes

/**
 * @brief Handler function / Entry point for the Temperature Sensing thread
 * 
 */
void tempSensorHandler (void);


/**
 * @brief Timer ISR to send a heartbeat message from the Temperature Sensing thread onto the message queue
 * 
 */
void tempHeartbeatTimerHandler (void);


/**
 * @brief Timer ISR to trigger reading a temperature value from the sensor
 * 
 */
void tempSensorTrigger (void);