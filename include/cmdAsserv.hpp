#pragma once

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
}

#include <cstdint>
#include <iostream>
#include "asservissement_interface.h"


class CmdAsserv : public asservissement_interface
{
private:
    int i2cFile;

public:
    CmdAsserv(int slave_address);
    void I2cSendData (uint8_t command, uint8_t* data, int length) override;
    void I2cReceiveData (uint8_t command, uint8_t* data, int length) override;
    ~CmdAsserv();
};