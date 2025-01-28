#pragma once

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
}

#include <iostream>
#include <cstdint>

class I2CDevice {
protected:
    int i2cFile;

public:

    // Returns diffent than 0 if error
    int I2cSendData (uint8_t command, uint8_t* data, int length);
    int I2cReceiveData (uint8_t command, uint8_t* data, int length);
    int I2cSendBlockReceiveData (uint8_t command, uint8_t* data, int length, uint8_t* out_data, int out_length);

protected:
    I2CDevice(int slave_address);
    ~I2CDevice();
};


/**
 * Read an int8_t from a buffer of bytes
 * @param buffer the buffer of bytes
 * @return the int8_t read from the buffer
 */
int8_t ReadInt8(uint8_t *buffer[]);

/**
 * Read an int16_t from a buffer of bytes
 * @param buffer the buffer of bytes
 * @return the int16_t read from the buffer
 */

int16_t ReadInt16(uint8_t *buffer[]);

/**
 * Read an int32_t from a buffer of bytes
 * @param buffer the buffer of bytes
 * @return the int32_t read from the buffer
 */

int32_t ReadInt32(uint8_t *buffer[]);

/**
 * Read an int64_t from a buffer of bytes
 * @param buffer the buffer of bytes
 * @return the int64_t read from the buffer
 */

int64_t ReadInt64(uint8_t *buffer[]);

/**
 * Write an int8_t to a buffer of bytes
 * @param buffer the buffer of bytes
 * @param val the int8_t to write
 */

void WriteInt8(uint8_t *buffer[], int8_t val);

/**
 * Write an int16_t to a buffer of bytes
 * @param buffer the buffer of bytes
 * @param val the int16_t to write
 */

void WriteInt16(uint8_t *buffer[], int16_t val);

/**
 * Write an int32_t to a buffer of bytes
 * @param buffer the buffer of bytes
 * @param val the int32_t to write
 */

void WriteInt32(uint8_t *buffer[], int32_t val);

/**
 * Write an int64_t to a buffer of bytes
 * @param buffer the buffer of bytes
 * @param val the int64_t to write
 */

void WriteInt64(uint8_t *buffer[], int64_t val);