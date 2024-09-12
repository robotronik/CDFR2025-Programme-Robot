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

/**
 * @brief Class to handle I2C communication with a device
 * 
 * This class is a base class to handle I2C communication with a device.
 * It provides some utility functions to handle I2C communication.
 * i2cFile is the file descriptor of the I2C device (e.g. /dev/i2c-1).
 * 
 */

class I2CDevice {
   protected:
    int i2cFile;
    /**
     * Generate a buffer of bytes from an array of integers
     * @param values the array of integers
     * @param length the length of the array
     * @param result the buffer to store the bytes
     */
    void generateBytes(int *values, size_t length, uint8_t *result);

    /**
     * Convert bytes to words
     * @param byteBuffer the buffer of bytes
     * @param wordBuffer the buffer of words
     * @param byteLength the length of the buffer of bytes
     */
    void bytesToWords(uint8_t *byteBuffer, int16_t *wordBuffer, size_t byteLength);

   protected:
    I2CDevice(int slave_address);
};