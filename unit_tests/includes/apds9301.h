/**
 * @file apds9301.h
 * @author Smitesh Modak and Ashish Tak
 * @brief : Header file for the library/HAL of the APDS-9301 Luminosity sensor
 * @version 0.1
 * @date 2019-03-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "i2c.h"


/**
 * @brief Perform the initialization routine for the luminosity sensor
 * 
 * @return int : Success/Failure (0/-1) of the initialization operation
 */
int initLumSensor(void);

/**
 * @brief Get the current Luminosity value from the sensor
 * 
 * @return float: Luminosity value
 */
float getLum(void);