/**
 * @file tmp102.h
 * @author Smitesh Modak and Ashish Tak
 * @brief : Header file containing function prototypes, enums and macros for the Temperature Sensor TMP102 library
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

#define TEMP_SENSOR_ADDR 0x48      // TMP102 address from datasheet

#define TEMP_ALERT_PIN   7

#define SHUTDOWN 0      // 0: Shutdown mode off
                        // 1: Shutdown mode on

#define ONE_SHOT 0      // 0: One shot/Conversion ready disabled
                        // 1: One shot/Conversion ready enabled
                        
#define EM  1           // 0: 12 bit resolution
                        // 1: 13 bit resolution

#define RATE CONV_FOUR  // Conversation rate based on enums defined below

#define ALERT_MODE 1    // 1: Interrupt Mode on, Comparator Mode off
                        // 0: Interrupt Mode off, Comparator Mode on (default)

#define POL 1           // define polarity, refer datasheet

#define LOW_TEMP  28.00f    // define low temp in degree Celsius

#define HIGH_TEMP 30.00f     // define high temp in degree Celsius

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
 * @return int returns 0 on success else -1
 */
int readConfig(void);

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
 * @return int returns 0 on success else -1
 */
int setLowTemp(void);

/**
 * @brief Set high limit register by providing setTempReg function with
 *        temperature value and register address
 * 
 * @return int returns 0 on success else -1
 */
int setHighTemp(void);

/**
 * @brief self test sensor by reading out default config register value 
 * 
 * @return int returns 0 on success else -1
 */
int selfTest(void);

#endif /* SRC_TMP102_H_ */