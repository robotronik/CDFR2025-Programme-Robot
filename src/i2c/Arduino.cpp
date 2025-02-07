#include "i2c/Arduino.hpp"
#include "utils/logger.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){}

#define CMD_MOVE_SERVO 0x01
#define CMD_READ_SENSOR 0x02
#define CMD_ENABLE_STEPPER 0x03
#define CMD_DISABLE_STEPPER 0x04
#define CMD_RGB_LED 0x05
#define CMD_PWM_LIDAR 0x06
#define CMD_MOVE_STEPPER 0x07
#define CMD_SET_STEPPER 0x08
#define CMD_GET_STEPPER 0x09


// [0;180]
void Arduino::moveServo(int ServoID, int position) {
    LOG_INFO("Arduino - Move servo #", ServoID, " to ", position);
    if (i2cFile == -1) return; // Emulation
    if (position < 0 || position > 180) {
        LOG_ERROR("Arduino - Servo position out of range");
        return;
    }
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt8(&ptr, ServoID);
    WriteUInt8(&ptr, position); // These could be simplified but for reading clarity
    if (I2cSendData(CMD_MOVE_SERVO, message, 2))
        LOG_ERROR("Arduino - Couldn't move servo");
}

bool Arduino::readSensor(int SensorID, bool& value){
    if (i2cFile == -1) return false; // Emulation
    uint8_t data;
    uint8_t message[1];
    uint8_t *ptr = message;
    WriteUInt8(&ptr, SensorID);
    if (I2cSendBlockReceiveData(CMD_READ_SENSOR, message, 1, &data, 1))
        return false;
    value = data;
    return true;
}

void Arduino::enableStepper(int StepperID) {
    LOG_DEBUG("Arduino - Enable Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    if (I2cSendData(CMD_ENABLE_STEPPER, (uint8_t*)&StepperID, 1))
        LOG_ERROR("Arduino - Couldn't enable Stepper");
}

void Arduino::disableStepper(int StepperID) {
    LOG_DEBUG("Arduino - Disable Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    if (I2cSendData(CMD_DISABLE_STEPPER, (uint8_t*)&StepperID, 1))
        LOG_ERROR("Arduino - Couldn't disable Stepper");
}

void Arduino::moveStepper(int32_t absPosition, int StepperID) {
    LOG_INFO("Arduino - Move Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [5];
    uint8_t *ptr = message;
    WriteUInt8(&ptr, StepperID);
    WriteInt32(&ptr, absPosition);
    if (I2cSendData(CMD_MOVE_STEPPER, message, 5))
        LOG_ERROR("Arduino - Couldn't move Stepper");
}


void Arduino::setStepper(int32_t absPosition, int StepperID){
    LOG_INFO("Arduino - Set Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [5];
    uint8_t *ptr = message;
    WriteUInt8(&ptr, StepperID);
    WriteInt32(&ptr, absPosition);
    if (I2cSendData(CMD_SET_STEPPER, message, 5))
        LOG_ERROR("Arduino - Couldn't set Stepper");
}

bool Arduino::getStepper(int32_t& absPosition, int StepperID){
    LOG_INFO("Arduino - Get Stepper #", StepperID);
    if (i2cFile == -1) return false; // Emulation
    uint8_t data[4];
    if (I2cSendBlockReceiveData(CMD_GET_STEPPER, (uint8_t*)&StepperID, 1, data, 4))
        return false;
    uint8_t* ptr = data;
    absPosition = ReadInt32(&ptr);
    LOG_DEBUG("Arduino - Stepper is at ", absPosition);
    return true;
}

void Arduino::RGB(int LED_ID, uint8_t mode, uint8_t r, uint8_t g, uint8_t b){
    if (i2cFile == -1) return; // Emulation
    uint8_t message [5];
    uint8_t *ptr = message;
    WriteUInt8(&ptr, LED_ID);
    WriteUInt8(&ptr, mode);
    if (mode != 2){
        WriteUInt8(&ptr, r);
        WriteUInt8(&ptr, g);
        WriteUInt8(&ptr, b);
        if (I2cSendData(CMD_RGB_LED, message, 5))
            LOG_ERROR("Arduino - Couldn't set LED");
    }
    else if (I2cSendData(CMD_RGB_LED, message, 2)) // Rainbow mode
        LOG_ERROR("Arduino - Couldn't set LED");
}

void Arduino::RGB_Solid(uint8_t R, uint8_t G, uint8_t B, int LED_ID){
    LOG_INFO("Arduino - Set RGB LED #", LED_ID, " to solid color (", (int)R, ", ", (int)G, ", ", (int)B, ")");
    RGB(LED_ID, 0, R, G, B);
}

void Arduino::RGB_Blinking(uint8_t R, uint8_t G, uint8_t B, int LED_ID){
    LOG_INFO("Arduino - Set RGB LED #", LED_ID, " to blinking color (", (int)R, ", ", (int)G, ", ", (int)B, ")");
    RGB(LED_ID, 1, R, G, B);
}

void Arduino::RGB_Rainbow(int LED_ID){
    LOG_INFO("Arduino - Set RGB LED #", LED_ID, " to rainbow");
    RGB(LED_ID, 2, 0, 0, 0);
}

void Arduino::SetLidarPWM(uint8_t val){
    if (I2cSendData(CMD_PWM_LIDAR, &val, 1))
        LOG_ERROR("Arduino - Couldn't set Lidar PWM");
}