#pragma once

#include <cstdint>
#include <iostream>
#include "i2c/I2CDevice.hpp"

class Arduino : public I2CDevice {
    using I2CDevice::I2CDevice;
   
   public:
    Arduino(int slave_address);

    // Functions return true if successfully executed
    void moveServo(int ServoID, int position);
    bool readSensor(int SensorID, bool& value);
    void moveStepper(int32_t absPosition, int StepperID);
    void setStepper(int32_t absPosition, int StepperID);
    bool getStepper(int32_t& absPosition, int StepperID);
    void enableStepper(int StepperID);
    void disableStepper(int StepperID);
    void RGB_Solid(uint8_t R, uint8_t G, uint8_t B, int LED_ID = 1);
    void RGB_Blinking(uint8_t R, uint8_t G, uint8_t B, int LED_ID = 1);
    void RGB_Rainbow(int LED_ID = 1);
    void SetLidarPWM(uint8_t val);
private:
    void RGB(int LED_ID, uint8_t mode, uint8_t r, uint8_t g, uint8_t b);
};