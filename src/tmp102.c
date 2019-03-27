/**
 * @file tmp102.c
 * @author Smitesh Modak and Ashish Tak
 * @brief  source file for Temperature Sensor TMP102
 * @version 0.1
 * @date 2019-03-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tmp102.h"
#include "i2c.h"

uint8_t tempReg = 0x00;             // Temperature register from datasheet
uint8_t configReg = 0x01;           // Config register from datasheet
uint8_t tlowReg = 0x02;             // Temperature Low register from datasheet
uint8_t thighReg= 0x03;             // Temperature High register from datasheet
uint8_t tempSensorAddr = 0x48;      // TMP102 address from datasheet

uint16_t readTempReg(void) {
    uint8_t buf[2] = {0};
    int16_t data;

    // Using I2C Read
    if (i2cRead(buf,2) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to read from the i2c bus.\n");
        return -1;
    } else {
        // Bit 0 of second byte will always be 0 in 12-bit readings and 1 in 13-bit
        if(buf[1]&0x01)	// 13 bit mode
        {
            // printf("Entered 13 bit mode");
            // Combine bytes to create a signed int
            data = ((buf[0]) << 5) | (buf[1] >> 3);
            // Temperature data can be + or -, if it should be negative,
            // convert 13 bit to 16 bit and use the 2s compliment.
            if(data > 0xFFF)
            {
                data |= 0xE000;
            }
        }
        else	// 12 bit mode
        {
            // Combine bytes to create a signed int 
            data = ((buf[0]) << 4) | (buf[1] >> 4);
            // Temperature data can be + or -, if it should be negative,
            // convert 12 bit to 16 bit and use the 2s compliment.
            if(data > 0x7FF)
            {
                data |= 0xF000;
            }
        }

    }
    return data;
}

float readTlow(void) {
    // Using I2C Write to specify which register to read
    if (i2cWrite(&tlowReg, 1) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus");
        return -1;
    }

    float tLowData = (float)readTempReg();
    return tLowData * 0.0625;        
}

float readThigh(void) {
    // Using I2C Write to specify which register to read
    if (i2cWrite(&thighReg, 1) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus");
        return -1;
    }

    float tHighData = (float)readTempReg();
    return tHighData * 0.0625;
}

float readTemp(void) {
    // Using I2C Write to specify which register to read
    if (i2cWrite(&tempReg, 1) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus");
        return -1;
    }

    float tempData = (float)readTempReg();
    return tempData * 0.0625; 
}

uint16_t readConfigReg(void) {
    uint8_t buf[2] = {0};
    uint16_t data;
    
    // Using I2C Write to specify which register to read
    if (i2cWrite(&configReg, 1) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus");
        return -1;
    }

    // Using I2C Read
    if (i2cRead(buf,2) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to read from the i2c bus.\n");
        return -1;
    } else {
        // Combine bytes to create a signed int 
        data = ((buf[0] << 8) | buf[1]);
    }
    return data;
}

int writeConfig(void) {
    uint8_t buf[3];
    
    volatile uint16_t configData = readConfigReg();

    uint8_t configByte0 = (configData & 0xFF00) >> 8;
    uint8_t configByte1 = (configData & 0x00FF);

    // Configure the sensor to go in and out of shutdown mode
    if (SHUTDOWN == 1)
        configByte0 |= (1 << 1);        // shutdown mode on
    else
        configByte0 &= ~(1 << 1);       // default shutdown mode
    
    // Set the EM operation
    if (EM == 1) 
        configByte1 |= (1 << 4);        // exteneded mode on
    else
        configByte1 &= ~(1 << 4);       // default EM operation

    // Set Conversion Rate
    configByte1 |= (RATE << 6);

    // Set polarity
    if(POL == 1) 
        configByte0 |= (1<<2);          
    else
        configByte0 &= ~(1<<2);         // default polarity 

    //Set Alert Mode
    if(ALERT_MODE == 1)
        configByte0 |= (1<<1);          // set to interrupt mode
    else
        configByte0 &= ~(1<<1);         // default alert mode

    if(FAULTS == FAULTS_ONE) {
        configByte0 &= ~(1<<4);
        configByte0 &= ~(1<<3);
    }        
    else if (FAULTS == FAULTS_TWO) {
        configByte0 &= ~(1<<4);
        configByte0 |= (1<<3);
    }
    else if (FAULTS == FAULTS_FOUR) {
        configByte0 |= (1<<4);
        configByte0 &= ~(1<<3);
    }
    else if (FAULTS == FAULTS_SIX) {
        configByte0 |= (1<<4);
        configByte0 |= (1<<3); 
    }

    buf[0] = configReg;
    buf[1] = configByte0;
    buf[2] = configByte1;

    printf("Write config byte0 %x \n", buf[1]);
    printf("Write config byte1 %x \n", buf[2]);

    // Using I2C Write to specify which register to read
    if (i2cWrite(buf, 3) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus");
        return -1;
    }

    return 0;
}

void readConfig(void) {
    uint16_t configReg = readConfigReg();
    printf("Read config %x \n",configReg);

    // Read the Fault bits
    uint8_t faults = (configReg >> 11) & (0x03);
    uint8_t consFaults = 0;
    if(faults == 0)
        consFaults = 1;
    else if (faults == 1)
        consFaults = 2;
    else if (faults == 2)
        consFaults = 4;
    else if (faults == 3)
        consFaults =6;

    // Read the EM operation
    bool emMode = (configReg >> 4);

    // Read Conversion Rate
    uint8_t convRate = (configReg >> 6) & (0x03);
    if(convRate == CONV_QUARTER)
        convRate = CONV_QUARTER;
    else if(convRate == CONV_ONE)
        convRate = CONV_ONE;
    else if(convRate == CONV_FOUR)
        convRate = CONV_FOUR;
    else if(convRate == CONV_EIGHT)
        convRate = CONV_EIGHT;
}

int setTempReg(float tempData, uint8_t cmd) {
    uint8_t buf[3];
    buf[0] = cmd;
    volatile uint16_t configData = readConfigReg();

    uint8_t configByte0 = (configData & 0xFF00) >> 8;
    uint8_t configByte1 = (configData & 0x00FF);

    tempData = tempData/0.0625;

     // Split temperature into separate bytes
    if(((configByte1 >> 4) & 0x1) == 1) {	// 13-bit mode
        buf[1] = ((int16_t)(tempData))>>5;
        buf[2] = ((int16_t)(tempData))<<3;
    }
    else	// 12-bit mode
    {
        buf[1] = ((int16_t)(tempData))>>4;
        buf[2] = ((int16_t)(tempData))<<4;
    }

    // Using I2C Write to specify which register to read
    if (i2cWrite(buf, 3) != 0) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus");
        return -1;
    }
    
    return 0;
}

void setLowTemp(void) {
    if(setTempReg(LOW_TEMP, tlowReg)!=0)
        printf("Error setting low temp");
}

void setHighTemp(void) {
    if(setTempReg(HIGH_TEMP, thighReg)!=0)
        printf("Error setting high temp");
}
