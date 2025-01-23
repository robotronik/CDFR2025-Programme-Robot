#pragma once

#include <cstdint>
#include <iostream>
#include "I2CDevice.hpp"

class Arduino : public I2CDevice {
    using I2CDevice::I2CDevice;
   
   public:
    Arduino(int slave_address);
    ~Arduino();

    // Functions return true if successfully executed
    void moveServo(int ServoID, uint8_t position);
    bool readSensor(int SensorID, bool& value);
    void moveStepper(int32_t absPosition, int StepperID);
    void setStepper(int32_t absPosition, int StepperID);
    bool getStepper(int32_t& absPosition, int StepperID);
    void enableStepper(int StepperID);
    void disableStepper(int StepperID);
    void ledOn(int LED_ID);
    void ledOff(int LED_ID);
};