/**
 * @file main.c
 * @author Smitesh Modak and Ashish Tak
 * @brief 
 * @version 0.1
 * @date 2019-03-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <mqueue.h>
#include "log.h"
#include "timer.h"
#include "tempSensor.h"
#include "queue.h"
#include "lumSensor.h"
#include "socket.h"
#include "i2c.h"
#include "tmp102.h"

int main () {
    const char line[500];
    size_t len = 0;
    ssize_t read;

    float data = readTemp();
    if (data == 80) {
        printf("Positive temperature test passed \n");
    }

    data = readTemp();
    if (data == 0) {
        printf("Zero temperature test passed \n");
    }

    char filename[] = "Test.txt";
    logInit(filename);
    LOG_INFO("Testing Log file %d \n", 100);
    filePtr = fopen(filename,"w+");

    while ((read = getline(&line, &len, filePtr)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }
    if (strcmp("Testing Log file 100", &line)) {
        printf("Line Read Properly \n");
    }
    
    return 0;
}