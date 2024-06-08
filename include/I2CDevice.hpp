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
    void generateBytes(int *values, size_t length, uint8_t *result);
    void bytesToWords(uint8_t *byteBuffer, int16_t *wordBuffer, size_t byteLength);

   protected:
    I2CDevice(int slave_address);
};