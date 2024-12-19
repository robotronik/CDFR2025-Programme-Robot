#pragma once

#include <cstdint>
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