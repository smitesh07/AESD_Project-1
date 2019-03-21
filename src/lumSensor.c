#include "lumSensor.h"

void lumSensorTrigger () {
    //Read luminosity from the sensor
    usleep(50); //Mocking the amount of time required for the read
    static uint32_t lum=0;
    printf("\nLum: %d percent", lum);
    fflush(stdout);
    lum+=2;
    return;
}


void *lumSensorHandler (void *arg) {
    uint32_t threadID= (pid_t)syscall(SYS_gettid);
    // initTimer(threadID, 2000000000, tempHeartbeatTimerHandler);
    initTimer(threadID, 1*1000000000, lumSensorTrigger);
    while (1) {
      //Call the function HERE to send the heartbeat signal to the message queue
      sleep(1);
    }
}