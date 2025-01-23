#include "Asserv.hpp"

Asserv::Asserv(int slave_address) : I2CDevice (slave_address){}

Asserv::~Asserv(){
    if (i2cFile >= 0) {
        close(i2cFile);
        std::cout << "I2C file closed successfully\n";
    }
}