/**
 * @file i2c.h
 * @author Smitesh Modak and Ashish Tak
 * @brief i2c header file containing function prototypes
 * @version 0.1
 * @date 2019-03-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SRC_I2C_H_
#define SRC_I2C_H_

/**
 * @brief       open the i2c bus
 * 
 * @return int  returns 0 on success else -1
 */
int i2cOpen(void);

/**
 * @brief       acquire the bus and talk to the slave whose address is passed
 * 
 * @param addr  slave address
 * @return int  returns 0 on success else -1
 */
int i2cCntrl(int addr);

/**
 * @brief       read data receive on the bus
 * 
 * @param buf   pointer to buffer in which received data has to be stored 
 * @param len   length of the buffer
 * @return int  returns 0 on success else -1
 */
int i2cRead(uint8_t *buf, unsigned int len);

/**
 * @brief       write data on the bus
 * 
 * @param buf   pointer to buffer from which data has to be written on the bus
 * @param len   length of the buffer
 * @return int  returns 0 on success else -1
 */
int i2cWrite(uint8_t *buf, unsigned int len);

/**
 * @brief       close the i2c bus
 * 
 * @return int  returns 0 on success else -1
 */
int i2cClose(void);

#endif /* SRC_I2C_H_ */