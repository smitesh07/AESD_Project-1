#include "lumSensor.h"


#define LUM_SLAVE_ADDRESS 0x39

//Threshold for the actual luminosity value to determine whether it's 'light' or 'dark'
#define DARKNESS_THRESHOLD 5


lumState currentState= LIGHT;
float lux;
luxUpdate *latestLux;
int slaveAddr = LUM_SLAVE_ADDRESS;


void lumSensorTrigger () {
    //Read luminosity from the sensor
    sem_wait(sem_i2c);
    if (i2cCntrl(slaveAddr)==-1) {
      latestLux->sensorConnected=false;
    }
    else {
      latestLux->sensorConnected=true;
      lux= getLum();
    }
    sem_post(sem_i2c);
    if (lux<DARKNESS_THRESHOLD && currentState==LIGHT) {
        enQueueForLog(WARN, "Change in luminosity level. It's now dark.", DARK); 
        currentState= DARK;
    }
    else if (lux>=DARKNESS_THRESHOLD && currentState==DARK) {
        enQueueForLog(WARN, "Change in luminosity level. It's now light.", LIGHT);
        currentState= LIGHT;
    }
    enQueueForLog(INFO, "Luminosity: ", lux);
    fflush(stdout);
    return;
}


void *lumSensorHandler (void *arg) {
    latestLux = (luxUpdate *)malloc(sizeof(latestLux));
    sem_wait(sem_i2c);
    if (i2cCntrl(slaveAddr)==-1) {
      latestLux->sensorConnected=false;
    }
    else {
      latestLux->sensorConnected=true;
      initLumSensor();
    }
    sem_post(sem_i2c);
    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    // initTimer(threadID, 2000000000, tempHeartbeatTimerHandler);
    initTimer(threadID, 1*1000000000, lumSensorTrigger);
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