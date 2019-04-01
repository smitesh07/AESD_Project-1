/**
 * @file lumSensor.c
 * @author Smitesh Modak and Ashish Tak
 * @brief : Source file for the luminosity sensing thread operations
 * @version 0.1
 * @date 2019-03-31
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "lumSensor.h"
#include <poll.h>
#include "SimpleGPIO.h"
#include "pollInt.h"

#define LUM_SLAVE_ADDRESS 0x39

//Threshold for the actual luminosity value to determine whether it's 'light' or 'dark'
#define DARKNESS_THRESHOLD 5

#define LUMINOSITY_SENSING_INTERVAL 3 //in seconds

poll_t pollFds;lumState currentState= LIGHT;
float lux;
luxUpdate *latestLux;
int slaveAddr = LUM_SLAVE_ADDRESS;


void lumSensorTrigger () {
    int ret;
    sem_wait(sem_i2c);
    i2cCntrl(slaveAddr);
    lux= getLum();
    sem_post(sem_i2c);
    if (lux<0) {
      latestLux->sensorConnected=false;
    }
    else {
      latestLux->sensorConnected=true;
      enQueueForLog(INFO, "Luminosity: ", lux);
      if (lux<DARKNESS_THRESHOLD && currentState==LIGHT) {
          enQueueForLog(WARN, "Change in luminosity level. It's now dark.", DARK);
          currentState= DARK;
      }
      else if (lux>=DARKNESS_THRESHOLD && currentState==DARK) {
          enQueueForLog(WARN, "Change in luminosity level. It's now light.", LIGHT);
          currentState= LIGHT;
      }
      fflush(stdout);
    }
    ret = poll(pollFds.poll_fds, 1, POLL_TIMEOUT);
    if (ret > 0) {
        // n = read(pollFds.f, &(pollFds.value), sizeof(pollFds.value));
        gpio_set_value(USR_LED1, 1);
        enQueueForLog(INFO, "Luminosity Sensor Interrupt Received",0);
        // lseek(pollFds.f, 0, SEEK_SET);
    }
    else {
        printf("\nInside else part lum");
        gpio_set_value(USR_LED1, 0);
        // lseek(pollFds.f, 0, SEEK_SET);
    }
    return;
}


void *lumSensorHandler (void *arg) {
    int ret;
    latestLux = (luxUpdate *)malloc(sizeof(latestLux));
    sem_wait(sem_i2c);
    i2cCntrl(slaveAddr);
    if (initLumSensor()==-1) {
      latestLux->sensorConnected=false;
    }
    else {
      latestLux->sensorConnected=true;
    }
    sem_post(sem_i2c);

    initTimer(LUMINOSITY_SENSING_INTERVAL*(uint64_t)1000000000, lumSensorTrigger);

    gpio_export(USR_LED1);
    gpio_set_dir(USR_LED1, OUTPUT_PIN);

    // Setup pin 7 for interrupt
    ret = gpio_export(LUM_ALERT_PIN);
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to export Luminosity Alert pin", 0);
    }

    ret = gpio_set_dir(LUM_ALERT_PIN, INPUT_PIN);
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to set direction for Luminosity Alert pin", 0);
    }

    ret = gpio_set_edge(LUM_ALERT_PIN, "falling");
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to set edge for Luminosity Alert pin", 0);
    }

    // Initialize polling for interrupt
    pollInit(LUM_ALERT_PIN, &pollFds);

    while (1) {
      lumHeartbeatFlag=true;
      sleep(1);
    }
}


luxUpdate * externReadLum() {
    latestLux->brightnessState= currentState;
    latestLux->lux=lux;
    return latestLux;
}
