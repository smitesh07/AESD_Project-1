/**
 * @file apds9301.c
 * @author Smitesh Modak and Ashish Tak
 * @brief : Implementation of the library/HAL of the APDS-9301 Luminosity sensor
 * @version 0.1
 * @date 2019-03-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "apds9301.h"


//Command values to read/write byte and word respectively from the sensor
#define CMD_RW_BYTE 0x80
#define CMD_RW_WORD 0xA0

#define BYTE_SIZE   1
#define WORD_SIZE   2

//Low and High thresholds for interrupt generation (corresponds to channel 0)
#define LOW_THRESHOLD   0x20
#define HIGH_THRESHOLD  0Xb07


//Register addresses for the APDS-9301 sensor
enum regAddress {
    CONTROL = 0x0,
    TIMING,
    THRESHLOWLOW,
    THRESHLOWHIGH,
    THRESHHIGHLOW,
    THRESHHIGHHIGH,
    INTERRUPT, 
    ID = 0xA,
    DATA0LOW = 0xC,
    DATA0HIGH,
    DATA1LOW,
    DATA1HIGH
}; 

char readBuffer[WORD_SIZE] = {0};
char writeBuffer[WORD_SIZE+1] = {0};


int initLumSensor() {
    //Write 00 to the control register to power down the device
    //Power it up again using 0x03 and read the value back
    writeBuffer[0]= CMD_RW_BYTE | CONTROL;
    writeBuffer[1]= 0x00;
    if (i2cWrite(writeBuffer, WORD_SIZE) ==-1) {
        return -1;
    }
    writeBuffer[1]= 0x03;
    i2cWrite(writeBuffer, WORD_SIZE);
    i2cRead(readBuffer, BYTE_SIZE);
    printf("\nControl register: 0x%x", readBuffer[0]);


    //Read the identification register
    writeBuffer[0]= CMD_RW_BYTE | ID;
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, BYTE_SIZE);
    printf("\nIdentification register: 0x%x", readBuffer[0]);

    //Read the timing register
    writeBuffer[0]= CMD_RW_BYTE | TIMING;
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, BYTE_SIZE);
    printf("\nTiming Register at startup: 0x%x", readBuffer[0]);
    //Set high gain and scale the integration time by 0.252 (101ms)
    writeBuffer[1]=0x11;
    i2cWrite(writeBuffer, WORD_SIZE);
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, BYTE_SIZE);
    printf("\nTiming Register after setting gain and integration scale: 0x%x", readBuffer[0]);

    //Set the Interrupt Threshold Registers
    writeBuffer[0]= CMD_RW_WORD | THRESHLOWLOW;
    writeBuffer[1]= LOW_THRESHOLD & 0xFF;
    writeBuffer[2]= (LOW_THRESHOLD & 0xFF00) >> 8;
    i2cWrite(writeBuffer, WORD_SIZE+1);
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, WORD_SIZE);
    uint16_t threshold= readBuffer[0]+ (((uint16_t)readBuffer[1])<<8);
    printf("\nLow Threshold: 0x%x",threshold);

    writeBuffer[0]= CMD_RW_WORD | THRESHHIGHLOW;
    writeBuffer[1]= HIGH_THRESHOLD & 0xFF;
    writeBuffer[2]= (HIGH_THRESHOLD & 0xFF00) >> 8;
    i2cWrite(writeBuffer, WORD_SIZE+1);    
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, WORD_SIZE);
    threshold= readBuffer[0]+ (((uint16_t)readBuffer[1])<<8);
    printf("\nHigh Threshold: 0x%x",threshold);

    //Enable the level interrupt in the Interrupt Control Register
    //And set persist for 5 cycles of integration
    writeBuffer[0]= CMD_RW_BYTE | INTERRUPT;
    writeBuffer[1]= 0x15;
    i2cWrite(writeBuffer, WORD_SIZE);
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, BYTE_SIZE);
    printf("\nInterrupt Control Reg (Level Interrupts enabled): 0x%x",readBuffer[0]);

    return 0;
}


float getLum() {
    uint16_t ch0, ch1;

    //Clear pending interrupt
    writeBuffer[0]= CMD_RW_BYTE | 0x40;
    if (i2cWrite(writeBuffer, BYTE_SIZE) == -1) {
        return -1;
    }

    //Read values from the 2 channels

    writeBuffer[0]= CMD_RW_WORD | DATA0LOW;
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, WORD_SIZE);
    ch0= readBuffer[0] + (uint16_t)(readBuffer[1]<<8); //Low byte is read from the sensor first

    writeBuffer[0]= CMD_RW_WORD | DATA1LOW;
    i2cWrite(writeBuffer, BYTE_SIZE);
    i2cRead(readBuffer, WORD_SIZE);
    ch1= readBuffer[0] + (uint16_t)(readBuffer[1]<<8);

    printf("\nCH0: 0x%x\nCH1: 0x%x\n", ch0, ch1);

    float lux, ratio;
    if (ch0 && ch1) {
        ratio= ch1/ch0;
    }

    //Calculate luminostiy based on the ratio
    if (ratio<=0.5) {
        lux=(0.0304*ch0)-(0.062*ch0*pow(ratio,1.4));
    }
    else if (ratio<=0.61) {
        lux=(0.0224*ch0)-(0.031*ch1);
    }
    else if (ratio<=0.8) {
        lux=(0.0128*ch0)-(0.0153*ch1);
    }
    else if (ratio<=1.3) {
        lux=(0.00146*ch0)-(0.00112*ch1);
    }
    else lux=0;

    return lux;
}

