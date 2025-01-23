#include "arduino.hpp"
#include "logger.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){}

Arduino::~Arduino(){
    if (i2cFile >= 0) {
        close(i2cFile);
        std::cout << "I2C file closed successfully\n";
    }
}

#define CMD_MOVE_SERVO 0x01
#define CMD_READ_SENSOR 0x02
#define CMD_ENABLE_STEPPER 0x03
#define CMD_DISABLE_STEPPER 0x04
#define CMD_LED_ON 0x05
#define CMD_LED_OFF 0x06
#define CMD_MOVE_STEPPER 0x07
#define CMD_SET_STEPPER 0x08
#define CMD_GET_STEPPER 0x09

// [0;180]
void Arduino::moveServo(int ServoID, int8_t position) {
    LOG_INFO("Arduino - Move servo #", ServoID, " to ", position);
    if (i2cFile == -1) return; // Emulation
    if (position < 0 || position > 180) {
        LOG_ERROR("Arduino - Servo position out of range");
        return;
    }
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteInt8(&ptr, ServoID);
    WriteInt8(&ptr, position); // These could be simplified but for reading clarity
    if (I2cSendData(CMD_MOVE_SERVO, (uint8_t*)&ServoID, 1))
        LOG_ERROR("Arduino - Couldn't move servo");
}

bool Arduino::readSensor(int SensorID, bool& value){
    if (i2cFile == -1) return false; // Emulation
    uint8_t data;
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteInt8(&ptr, SensorID);
    WriteInt8(&ptr, value);
    if (I2cSendBlockReceiveData(CMD_READ_SENSOR, message, 2, &data, 1))
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

void Arduino::ledOn(int LED_ID) {
    if (i2cFile == -1) return; // Emulation
    if (I2cSendData(CMD_LED_ON, (uint8_t*)&LED_ID, 1))
        LOG_ERROR("Arduino - Couldn't set led on");
}

void Arduino::ledOff(int LED_ID) {
    if (i2cFile == -1) return; // Emulation
    if (I2cSendData(CMD_LED_ON, (uint8_t*)&LED_ID, 1))
        LOG_ERROR("Arduino - Couldn't set led off");
}

void Arduino::moveStepper(int32_t absPosition, int StepperID) {
    LOG_INFO("Arduino - Move Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [5];
    uint8_t *ptr = message;
    WriteInt8(&ptr, StepperID);
    WriteInt32(&ptr, absPosition);
    if (I2cSendData(CMD_MOVE_STEPPER, message, 5))
        LOG_ERROR("Arduino - Couldn't move Stepper");
}


void Arduino::setStepper(int32_t absPosition, int StepperID){
    LOG_INFO("Arduino - Set Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [5];
    uint8_t *ptr = message;
    WriteInt8(&ptr, StepperID);
    WriteInt32(&ptr, absPosition);
    if (I2cSendData(CMD_SET_STEPPER, message, 5))
        LOG_ERROR("Arduino - Couldn't set Stepper");
}

bool Arduino::getStepper(int32_t& absPosition, int StepperID){
    LOG_INFO("Arduino - Get Stepper #", StepperID);
    if (i2cFile == -1) return false; // Emulation
    uint8_t data[4];
    uint8_t message [1];
    uint8_t *ptr = message;
    WriteInt8(&ptr, StepperID);
    if (I2cSendBlockReceiveData(CMD_GET_STEPPER, message, 1, data, 4))
        return false;
    ptr = data;
    absPosition = ReadInt32(&ptr);
    return true;
}