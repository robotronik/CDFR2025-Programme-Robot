#include "I2CDevice.hpp"
#include <string.h> // Include for memcpy

I2CDevice::I2CDevice(int slave_address){
    int adapter_nr = 1; /* probably dynamically determined */
    char filename[20];

    if (slave_address == -1) // I2C Emulation
    {
        std::cout << "Emulating I2C\n";
        i2cFile = -1;
    }
    else{
        snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
        i2cFile = open(filename, O_RDWR);
        if (i2cFile < 0) {
            /* ERROR HANDLING; you can check errno to see what went wrong */
            std::cout << "Couldn't open I2C file\n";
            exit(1);
        }

        if (ioctl(i2cFile, I2C_SLAVE, slave_address) < 0) {
            std::cout << "ioctl failed\n";
            close(i2cFile);
            exit(1);
        }
    }
}

I2CDevice::~I2CDevice(){
    if (i2cFile >= 0) {
        close(i2cFile);
        std::cout << "I2C file closed successfully\n";
    }
}

int I2CDevice::I2cSendData (uint8_t command, uint8_t* data, int length){
    if (i2cFile >= 0){
        if(length != 0){
            return (i2c_smbus_write_i2c_block_data(i2cFile, command, length, data));
        }
        else{
            return (i2c_smbus_write_byte(i2cFile, command));
        }
    }
    return 0;
}


int I2CDevice::I2cReceiveData (uint8_t command, uint8_t* data, int length){
    if (i2cFile >= 0){
        if (i2c_smbus_write_byte(i2cFile, command))
            return -1;
        i2c_smbus_read_byte(i2cFile);
        if (read(i2cFile, data, length) != length)
            return -1;
    }
    else{
        // Emulate I2C by return data full of 0x00
        for (int i = 0; i < length; i++){
            data[i] = 0x00;
        }
    }
    return 0;
}

int I2CDevice::I2cSendBlockReceiveData (uint8_t command, uint8_t* data, int length, uint8_t* out_data, int out_length){

    if (i2cFile >= 0){
        if(length != 0){
            if (i2c_smbus_write_i2c_block_data(i2cFile, command, length, data))
                return -1;
        }
        else{
            if (i2c_smbus_write_byte(i2cFile, command))
                return -1;
        }
        if (read(i2cFile, out_data, out_length) != out_length)
            return -1;
    }
    else{
        // Emulate I2C by return data full of 0x00
        for (int i = 0; i < out_length; i++){
            out_data[i] = 0x00;
        }
    }
    return 0;
}

// Pointer to an array pointer
int8_t ReadInt8(uint8_t *buffer[]){
    int8_t val;
    memcpy(&val, *buffer, 1);
    *buffer += 1;
    return val;
}

int16_t ReadInt16(uint8_t *buffer[]){
    int16_t val;
    memcpy(&val, *buffer, 2);
    *buffer += 2;
    return val;
}

int32_t ReadInt32(uint8_t *buffer[]){
    int32_t val;
    memcpy(&val, *buffer, 3);
    *buffer += 4;
    return val;
}


int64_t ReadInt64(uint8_t *buffer[]){
    int64_t val;
    memcpy(&val, *buffer, 4);
    *buffer += 8;
    return val;
}

// Writes val at buffer and increments the buffer
void WriteInt8(uint8_t *buffer[], int8_t val){
    memcpy(*buffer, &val, 1);
    *buffer += 1;
}

void WriteInt16(uint8_t *buffer[], int16_t val){
    memcpy(*buffer, &val, 2);
    *buffer += 2;
}

void WriteInt32(uint8_t *buffer[], int32_t val){
    memcpy(*buffer, &val, 3);
    *buffer += 4;
}

void WriteInt64(uint8_t *buffer[], int64_t val){
    memcpy(*buffer, &val, 4);
    *buffer += 8;
}