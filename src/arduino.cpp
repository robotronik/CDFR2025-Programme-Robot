#include "arduino.hpp"
#include "logger.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){}

Arduino::~Arduino(){
    if (i2cFile >= 0) {
        close(i2cFile);
        std::cout << "I2C file closed successfully\n";
    }
}

// [0;180]
void Arduino::moveServo(int ServoID, int8_t position) {
    LOG_INFO("Arduino - Move servo #", ServoID, " to ", position);
    int length = 2;  // Nb of bytes to send
    uint8_t message[2];
    int values[] = {position};

    if (i2cFile == -1) return; // Emulation
    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t) ServoID, length, message)){
        LOG_ERROR("Arduino - Couldn't move servo");
    }
}

bool Arduino::readSensor(int SensorID, bool& value){
    uint8_t buffer[2] = {0,0};
    int command = 100 + (SensorID -1);

    if (i2cFile == -1) return false; // Emulation
    i2c_smbus_write_byte(i2cFile, command);
    int bytesRead = read(i2cFile, buffer, 2);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    int state = resultMSB<<8 | resultLSB;
    value = state != 0;

    if (bytesRead != 2) {
        LOG_ERROR("Arduino - Couldn't read sensor #", SensorID);
        return false; 
    }
    return true;
}

void Arduino::enableStepper(int StepperID) {
    LOG_DEBUG("Arduino - Enable Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    if(i2c_smbus_write_byte(i2cFile, (StepperID-1)*2 + 21)){
        LOG_ERROR("Arduino - Couldn't enable Stepper");
    }
}

void Arduino::disableStepper(int StepperID) {
    LOG_DEBUG("Arduino - Disable Stepper #", StepperID);
    if (i2cFile == -1) return; // Emulation
    if(i2c_smbus_write_byte(i2cFile,(StepperID-1)*2 + 22)){
        LOG_ERROR("Arduino - Couldn't disable Stepper");
    }
}

void Arduino::ledOn(int LED_ID) {
    if (i2cFile == -1) return; // Emulation
    if(i2c_smbus_write_byte(i2cFile, (LED_ID-1)*2 + 31)){
        LOG_ERROR("Arduino - Couldn't turn on the led");
    }
}

void Arduino::ledOff(int LED_ID) {
    if (i2cFile == -1) return; // Emulation
    if(i2c_smbus_write_byte(i2cFile, (LED_ID-1)*2 + 32)){
        LOG_ERROR("Arduino - couldn't turn off the led");
    }
}

void Arduino::moveStepper(int32_t absPosition, int StepperID) {
    LOG_INFO("Arduino - Move Stepper #", absPosition);
    int length = 2;  // Nb of bytes to send
    uint8_t message[2];
    int values[] = {absPosition};

    if (i2cFile == -1) return; // Emulation
    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)(10+StepperID), length, message)){
        LOG_ERROR("Arduino - Couldn't move Stepper");
    }
}


void Arduino::setStepper(int32_t absPosition, int StepperID){
    // TODO
}

bool Arduino::getStepper(int32_t& absPosition, int StepperID){
    // TODO
    return false;
}