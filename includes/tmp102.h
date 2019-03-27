/**
 * @file tmp102.h
 * @author Smitesh Modak and Ashish Tak
 * @brief header file containing function prototypes, enums and macros
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

#define ALERT_MODE 1    // 1: Interrupt Mode on, Comparator Mode off
                        // 0: Interrupt Mode off, Comparator Mode on (default)

#define POL 1           // define polarity, refer datasheet

#define LOW_TEMP  50.00f    // define low temp in degree Celsius

#define HIGH_TEMP 0.00f     // define high temp in degree Celsius

#define FAULTS FAULTS_ONE   // Consecutive faults based on enums defined below

// Conversion rate enums
typedef enum{
    CONV_QUARTER = 0,   // 0.25 Hz
    CONV_ONE,           // 1 Hz
    CONV_FOUR,          // 4 Hz(default)
    CONV_EIGHT          // 8 Hz
} CONVERSION_RATE;

// Consecutive faults enum
typedef enum{
    FAULTS_ONE = 0,
    FAULTS_TWO,
    FAULTS_FOUR,
    FAULTS_SIX
}CONS_FAULTS;

/**
 * @brief Read temperature register
 * 
 * @return uint16_t return temperature data in binary form
 */
uint16_t readTempReg(void);

/**
 * @brief Read low limit temperature register
 * 
 * @return float returns low limit temperature in degree celsius
 */
float readTlow(void);

/**
 * @brief Read high limit temperature register
 * 
 * @return float returns high limit temperature in degree celsius
 */
float readTemp(void);

/**
 * @brief Read config register
 * 
 * @return uint16_t Returns config register value in binary form
 */
uint16_t readConfigReg(void);

/**
 * @brief write to config register
 * 
 * @return int returns 0 on success else -1
 */
int writeConfig(void);

/**
 * @brief read config register by calling read config reg and print out necessary data
 * 
 */
void readConfig(void);

/**
 * @brief Set the Temp Register
 * 
 * @param tempData  Temperate value to be written
 * @param cmd       Temperature low/high limit register ptr address
 * @return int      returns 0 on success else -1
 */
int setTempReg(float tempData, uint8_t cmd);

/**
 * @brief Set low limit register by providing setTempReg function with
 *        temperature value and register address
 * 
 */
void setLowTemp(void);

/**
 * @brief Set high limit register by providing setTempReg function with
 *        temperature value and register address
 * 
 */
void setHighTemp(void);


#endif /* SRC_TMP102_H_ */