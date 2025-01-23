#include "I2CDevice.hpp"

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

// Takes in input an array of ints to convert to an array of uint8_t LSB first then MSB (Little Endian)
void I2CDevice::generateBytes(int *values, size_t length, uint8_t *result) {
    for (size_t i = 0; i < (length/2); i++) {
        uint8_t resultMSB, resultLSB;
        resultMSB = (uint8_t)(values[i] & 0xFF);
        resultLSB = (uint8_t)((values[i] >> 8) & 0xFF);
        result[2 * i] = resultLSB;
        result[2 * i + 1] = resultMSB;
    }
}

void I2CDevice::bytesToWords(uint8_t *byteBuffer, int16_t *wordBuffer, size_t byteLength){
    // Fills the word buffer with a mix of byte buffer and signs the numbers
   
    uint8_t resultMSB, resultLSB;
    for (size_t i = 0; i < (byteLength / 2); i++){
        resultMSB = byteBuffer[2*i];
        resultLSB = byteBuffer[2*i + 1];
        wordBuffer[i] = (int16_t) (resultMSB >> 8 | resultLSB);
    }
}

void I2CDevice::I2cSendData (uint8_t command, uint8_t* data, int length){
    if (i2cFile >= 0){
        if(length != 0){
            i2c_smbus_write_i2c_block_data(i2cFile, command, length, data);
        }
        else{
            i2c_smbus_write_byte(i2cFile, command);
        }
    }
}


void I2CDevice::I2cReceiveData (uint8_t command, uint8_t* data, int length){
    if (i2cFile >= 0){
        i2c_smbus_write_byte(i2cFile, command);
        read(i2cFile, data, length);
    }
    else{
        // Emulate I2C by return data full of 0x00
        for (int i = 0; i < length; i++){
            data[i] = 0x00;
        }
    }
}