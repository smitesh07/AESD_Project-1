/**
 * @file tempSensor.c
 * @author Ashish Tak and Smitesh Modak
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
#include <time.h>

#define TEMPERATURE_SENSING_INTERVAL 3 //in seconds

poll_t pollFds;
float tempData;
timer_t tempTimerid;
bool tempHeartbeatFlag;

tempUpdate *latestTemp;

void tempSensorTrigger () {
    int n;
    int ret;

    //Acquire the semaphore
    sem_wait(sem_i2c);

    i2cCntrl(TEMP_SENSOR_ADDR);

    tempData = readTemp();

    // Release the semaphore
    sem_post(sem_i2c);

    if(UNIT == CELSIUS)
      tempData = tempData;
    else if (UNIT == FAHRENHEIT)
      tempData = (tempData * 9 / 5) + 32;
    else if (UNIT == KELVIN)
      tempData = tempData +  273.15;

    if (tempData == -1) {
      latestTemp->sensorConnected=false;
      gpio_set_value(USR_LED0, 1);
      usleep(500000);
      gpio_set_value(USR_LED0, 0);
    } else {
      latestTemp->sensorConnected=true;
      // printf("Temperature Value in deg C: %f",tempData);
      if(UNIT == CELSIUS)
        enQueueForLog(INFO, "Temperature Value in deg C: ",tempData);
      else if (UNIT == FAHRENHEIT)
        enQueueForLog(INFO, "Temperature Value in deg F: ",tempData);
      else if (UNIT == KELVIN)
        enQueueForLog(INFO, "Temperature Value in kelvin: ",tempData);
    }

    // printf("Waiting");
    ret = poll(pollFds.poll_fds, 1, POLL_TIMEOUT);
    if (ret > 0) {
        // n = read(pollFds.f, &(pollFds.value), sizeof(pollFds.value));
        gpio_set_value(USR_LED0, 1);
        enQueueForLog(INFO, "Temperature Sensor Interrupt Received",0);
        lseek(pollFds.f, 0, SEEK_SET);
    }
    else {
        gpio_set_value(USR_LED0, 0);
        lseek(pollFds.f, 0, SEEK_SET);
    }

    return;
}


void *tempSensorHandler (void *arg) {
    int ret;
    latestTemp = (tempUpdate *)malloc(sizeof(latestTemp));

    //Acquire the semaphore
    sem_wait(sem_i2c);
    i2cCntrl(TEMP_SENSOR_ADDR);
    if (selfTest()==-1) {
      latestTemp->sensorConnected=false;
      enQueueForLog(ERROR, "Temperature Sensor Thread terminating.. Startup Test failed!! ", 0);
      //The latest Temp structure is retained in memory to cater to the Remote Socket Requests
      pthread_exit();
    }
    else {
      LOG_INFO("Temperature Sensor Startup test successful");
    }
    ret = writeConfig();
    setHighTemp();
    setLowTemp();
    // Release the semaphore
    sem_post(sem_i2c);

    tempTimerid = initTimer(TEMPERATURE_SENSING_INTERVAL*(uint64_t)1000000000, tempSensorTrigger);

    if (ret != 0) {
      enQueueForLog(ERROR, "Could not configure temperature sensor",0);
      latestTemp->sensorConnected=false;
    } else {
      latestTemp->sensorConnected=true;
    }

    gpio_export(USR_LED0);
    gpio_set_dir(USR_LED0, OUTPUT_PIN);

    // Setup pin 7 for interrupt
    ret = gpio_export(TEMP_ALERT_PIN);
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to export Temperature Alert pin", 0);
    }

    ret = gpio_set_dir(TEMP_ALERT_PIN, INPUT_PIN);
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to set direction for Temperature Alert pin", 0);
    }

    ret = gpio_set_edge(TEMP_ALERT_PIN, "rising");
    if (ret != 0) {
      enQueueForLog(ERROR, "Unable to set edge for Temperature Alert pin", 0);
    }

    // Initialize polling for interrupt
    pollInit(TEMP_ALERT_PIN, &pollFds);

    while (1) {
      //Periodically set the heartbeat flag to be checked by main()
      tempHeartbeatFlag = true;
      //Main sets this global flag on receiving the SIGINT signal from user
      if (terminateSignal) {
        enQueueForLog(WARN, "Termination signal received to Temperature sensing thread.", 0);
        deQueueFromLog();
        //Cleanup procedure for Temperature sensing thread
        timer_delete(tempTimerid);
        free(latestTemp);
        break;
      }
      sleep(1);
    }
}


tempUpdate * externReadTemp() {
    latestTemp->temp=tempData;
    return latestTemp;
}
