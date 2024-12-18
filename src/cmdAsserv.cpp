#include "cmdAsserv.hpp"


CmdAsserv::CmdAsserv(int slave_address){
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
        close(i2cFile); // Clean up before throwing
        exit(1);
    }        
}

CmdAsserv::~CmdAsserv(){
    if (i2cFile >= 0) {
        close(i2cFile);
        std::cout << "I2C file closed successfully\n";
    }
}

void CmdAsserv::I2cSendData (uint8_t command, uint8_t* data, int length){
    if(length != 0){
        i2c_smbus_write_i2c_block_data(i2cFile, command, length, data);
    }
    else{
        i2c_smbus_write_byte(i2cFile, command);
    }
}


void CmdAsserv::I2cReceiveData (uint8_t command, uint8_t* data, int length){
    i2c_smbus_write_byte(i2cFile, command);
    read(i2cFile, data, length);
}

