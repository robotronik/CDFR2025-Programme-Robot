#pragma once

#include <cstdint>

class I2CDevice {
protected:
    int i2cFile;

public:

    // Returns diffent than 0 if error
    int I2cSendData (uint8_t command, uint8_t* data, int length, int tries = 5);
    int I2cReceiveData (uint8_t command, uint8_t* data, int length, int tries = 5);
    int I2cSendBlockReceiveData (uint8_t command, uint8_t* data, int length, uint8_t* out_data, int out_length, int tries = 5);

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
int16_t ReadInt16(uint8_t *buffer[]);
int32_t ReadInt32(uint8_t *buffer[]);
int64_t ReadInt64(uint8_t *buffer[]);
uint8_t ReadUInt8(uint8_t *buffer[]);
uint16_t ReadUInt16(uint8_t *buffer[]);
uint32_t ReadUInt32(uint8_t *buffer[]);
uint64_t ReadUInt64(uint8_t *buffer[]);

void WriteInt8(uint8_t *buffer[], int8_t val);
void WriteInt16(uint8_t *buffer[], int16_t val);
void WriteInt32(uint8_t *buffer[], int32_t val);
void WriteInt64(uint8_t *buffer[], int64_t val);
void WriteUInt8(uint8_t *buffer[], uint8_t val);
void WriteUInt16(uint8_t *buffer[], uint16_t val);
void WriteUInt32(uint8_t *buffer[], uint32_t val);
void WriteUInt64(uint8_t *buffer[], uint64_t val);