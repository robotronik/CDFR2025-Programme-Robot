#pragma once

#include <cstdint>
#include <iostream>
#include "I2CDevice.hpp"

class Arduino : public I2CDevice {
    using I2CDevice::I2CDevice;
   
   public:
    Arduino(int slave_address);
    int servoPosition(int servoNb, int position);
    int readCapteur(int capteurNumber, int &state);
    int moveStepper(int absPosition, int stepperNb);
    int enableStepper(int stepperNb);
    int disableStepper(int stepperNb);
    int ledOn(int LedNb);
    int ledOff(int LedNb);
};