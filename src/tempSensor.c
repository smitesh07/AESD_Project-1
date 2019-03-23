#include "tempSensor.h"
#include "queue.h"
#include "log.h"

/*
void tempHeartbeatTimerHandler () {
  //Send a heartbeat message onto the message queue
  printf("\nTemp Sensor thread heartbeat timeout");
  fflush(stdout);
  return;
}
*/


void tempSensorTrigger () {
    //Read temperature from the sensor
    usleep(50); //Mocking the amount of time required for the read
    static uint32_t temp=0;
    // printf("\nTemp: %d degrees", temp++);
    enQueueForLog(INFO, "Temp Value: ", temp++);
    fflush(stdout);
    return;
}


void *tempSensorHandler (void *arg) {
    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    // initTimer(threadID, 2000000000, tempHeartbeatTimerHandler);
    initTimer(threadID, 1*1000000000, tempSensorTrigger);
    while (1) {
      //Call the function HERE to send the heartbeat signal to the message queue
      sleep(1);
    }
}