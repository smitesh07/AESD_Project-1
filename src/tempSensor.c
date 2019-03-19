#include "tempSensor.h"


void tempHeartbeatTimerHandler () {
  //Send a heartbeat message onto the message queue
  printf("\nTemp Sensor thread heartbeat timeout");
  return;
}


void tempSensorTrigger () {
    //Read temperature from the sensor
    printf("\nTemp: x degrees");
    return;
}


void tempSensorHandler () {
    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    initTimer(threadID, 2000000000, tempHeartbeatTimerHandler);
    initTimer(threadID, 1000000000, tempSensorTrigger);
    while (1) {
      sleep(1);
    }
}