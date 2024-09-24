#include "commandesAsservissement.hpp"


commandesAsservissement::commandesAsservissement(int slave_address){
    int adapter_nr = 1; /* probably dynamically determined */
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    i2cFile = open(filename, O_RDWR);
    if (i2cFile < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        std::cout << "Couldn't open I2C file\n";
        exit(1);
    }

    if (ioctl(i2cFile, I2C_SLAVE, slave_address) < 0) {
        std::cout << "ioctl failed\n";
        exit(1);
    }
}

commandesAsservissement::~commandesAsservissement()
{
}

void commandesAsservissement::I2cSendData (uint8_t command, uint8_t* data, int length){
    if(length == 0){
        i2c_smbus_write_i2c_block_data(i2cFile, command, length, data);
    }
    else{
        i2c_smbus_write_byte(i2cFile, command);
    }
}


void commandesAsservissement::I2cReceiveData (uint8_t command, uint8_t* data, int length){
    i2c_smbus_write_byte(i2cFile, command);
    int bytesRead = read(i2cFile, data, length);
}

