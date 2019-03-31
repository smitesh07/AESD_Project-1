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


#define LUM_SLAVE_ADDRESS 0x39

//Threshold for the actual luminosity value to determine whether it's 'light' or 'dark'
#define DARKNESS_THRESHOLD 5

#define LUMINOSITY_SENSING_INTERVAL 1 //in seconds


lumState currentState= LIGHT;
float lux;
luxUpdate *latestLux;
int slaveAddr = LUM_SLAVE_ADDRESS;


void lumSensorTrigger () {
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
    return;
}


void *lumSensorHandler (void *arg) {
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
    initTimer(LUMINOSITY_SENSING_INTERVAL*1000000000, lumSensorTrigger);
    while (1) {
      //Call the function HERE to send the heartbeat signal to the message queue
      sleep(1);
    }
}


luxUpdate * externReadLum() {
    latestLux->brightnessState= currentState;
    latestLux->lux=lux;
    return latestLux;
}