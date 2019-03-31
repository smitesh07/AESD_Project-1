/**
 * @file tempSensor.c
 * @author Ashish Tak
 * @brief : Source file for the temperature sensing thread operations
 * @version 0.1
 * @date 2019-03-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "tempSensor.h"
#include "queue.h"
#include "log.h"
#include "i2c.h"
#include "tmp102.h"
#include "SimpleGPIO.h"
#include "pollInt.h"
#include <poll.h>

#define TEMPERATURE_SENSING_INTERVAL 1 //in seconds

poll_t pollFds;
float tempData;

tempUpdate *latestTemp;

void tempSensorTrigger () {
    int n;
    int ret;

    //Acquire the semaphore
    sem_wait(sem_i2c);

    i2cCntrl(TEMP_SENSOR_ADDR);

    tempData = readTemp();
    if(UNIT == CELSIUS)
      tempData = tempData;
    else if (UNIT == FAHRENHEIT)
      tempData = (tempData * 9 / 5) + 32;
    else if (UNIT == KELVIN)
      tempData = tempData +  273.15;

    if (tempData == -1) {
      latestTemp->sensorConnected=false;
    } else {
      latestTemp->sensorConnected=true;
      printf("Temperature Value in deg C: %f",tempData);
      if(UNIT == CELSIUS)
        enQueueForLog(INFO, "Temperature Value in deg C: ",tempData);
      else if (UNIT == FAHRENHEIT)
        enQueueForLog(INFO, "Temperature Value in deg F: ",tempData);
      else if (UNIT == KELVIN)
        enQueueForLog(INFO, "Temperature Value in kelvin: ",tempData);
    }

    ret = poll(pollFds.poll_fds, 1, POLL_TIMEOUT);
    if (ret > 0) {
        // n = read(pollFds.f, &(pollFds.value), sizeof(pollFds.value));
        gpio_set_value(USR_LED, 1);
        sleep(1);
        gpio_set_value(USR_LED, 0);
        enQueueForLog(INFO, "Temperature Sensor Interrupt Received",0);
        lseek(pollFds.f, 0, SEEK_SET);
    }

    // Release semaphore
    sem_post(sem_i2c);

    return;
}


void *tempSensorHandler (void *arg) {
    initTimer(TEMPERATURE_SENSING_INTERVAL*1000000000, tempSensorTrigger);

    int ret;
    latestTemp = (tempUpdate *)malloc(sizeof(latestTemp));

    //Acquire the semaphore
    sem_wait(sem_i2c);
    i2cCntrl(TEMP_SENSOR_ADDR);
    ret = writeConfig();
    if (ret != 0) {
      enQueueForLog(ERROR, "Could not configure temperature sensor",0);
      latestTemp->sensorConnected=false;
    } else {
      latestTemp->sensorConnected=true;
    }
    // Release semaphore
    sem_post(sem_i2c);

    // Setup pin 7 for interrupt
    ret = gpio_export(TEMP_ALERT_PIN);
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to export Temperature Alert pin", 0);
    }

    ret = gpio_set_dir(TEMP_ALERT_PIN, INPUT_PIN);
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to set direction for Temperature Alert pin", 0);
    }

    ret = gpio_set_edge(7, "falling");
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to set edge for Temperature Alert pin", 0);
    }
    
    // Initialize polling for interrupt
    pollInit(7, &pollFds);

    while (1) {
      //Call the function HERE to send the heartbeat signal to the message queue
      sleep(1);
    }
}


tempUpdate * externReadTemp() {
    latestTemp->temp=tempData;
    return latestTemp;
}