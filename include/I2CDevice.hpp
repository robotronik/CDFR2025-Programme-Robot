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

int8_t ReadInt8(uint8_t *buffer[]);
int16_t ReadInt16(uint8_t *buffer[]);
int32_t ReadInt32(uint8_t *buffer[]);
int64_t ReadInt64(uint8_t *buffer[]);

void WriteInt8(uint8_t *buffer[], int8_t val);
void WriteInt16(uint8_t *buffer[], int16_t val);
void WriteInt32(uint8_t *buffer[], int32_t val);
void WriteInt64(uint8_t *buffer[], int64_t val);