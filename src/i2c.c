/**
 * @file i2c.c
 * @author Smitesh Modak and Ashish Tak
 * @brief source file to handle i2c operations
 * @version 0.1
 * @date 2019-03-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "i2c.h"

static int file;

int i2cOpen(void){
    char fileName[40];
    sprintf(fileName,"/dev/i2c-2");
    if ((file = open(fileName,O_RDWR)) < 0) {
        perror("Failed to open the bus.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        return -1;
    }

    return 0;
}

int i2cCntrl(int addr) {
    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        perror("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        return -1;
    }

    return 0;
}

int i2cRead(uint8_t *buf, unsigned int len) {
    // Using I2C Read
    if (read(file,buf,len) != len) {
        /* ERROR HANDLING: i2c transaction failed */
        perror("Failed to read from the i2c bus.\n");
        return -1;
    }

    return 0;
}

int i2cWrite(uint8_t *buf, unsigned int len) {
    if (write(file,buf,len) != len) {
        /* ERROR HANDLING: i2c transaction failed */
        perror("Failed to write to the i2c bus.\n");
        return -1;
    }

    return 0;
}

int i2cClose(void) {
    if(close(file) < 0) {
        perror("Error closing i2c bus");
        return -1;
    }
    
    return 0;
}