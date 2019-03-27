/**
 * @file i2c.h
 * @author Smitesh Modak and Ashish Tak
 * @brief i2c header file containing function prototypes, enums and macros
 * @version 0.1
 * @date 2019-03-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SRC_TMP102_H_
#define SRC_TMP102_H_

#include <stdlib.h>
#include <stdint.h>

#define SHUTDOWN 1      // 0: Shutdown mode off
                        // 1: Shutdown mode on
                        
#define EM  1           // 0: 12 bit resolution
                        // 1: 13 bit resolution

#define RATE CONV_FOUR  // Conversation rate based on enums defined below

typedef enum{
    CONV_QUARTER = 0,   // 0.25 Hz
    CONV_ONE,           // 1 Hz
    CONV_FOUR,          // 4 Hz(default)
    CONV_EIGHT          // 8 Hz
} CONVERSION_RATE;

#endif /* SRC_TMP102_H_ */