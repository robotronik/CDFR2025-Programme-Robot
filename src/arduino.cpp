#include "arduino.hpp"
#include "logger.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){}


int Arduino::servoPosition(int servoNb, int position) {
    LOG_INFO("servo ",servoNb," postion : ",position);
    int length = 2;  // Nb of bytes to send
    uint8_t message[2];
    int values[] = {position};

    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t) servoNb, length, message)){
        LOG_ERROR("couldn't write servo postion");
    }
    return 0;
}

int Arduino::readCapteur(int capteurNumber, int &state){
    uint8_t buffer[2] = {0,0};
    int command = 100 + (capteurNumber -1);

    i2c_smbus_write_byte(i2cFile, command);
    int bytesRead = read(i2cFile, buffer, 2);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    state = resultMSB<<8 | resultLSB;

    if (bytesRead != 2) {
        LOG_ERROR("couldn't read capteur n°",capteurNumber);
        return -1; 
    }
    
    return 0;
}

int Arduino::enableStepper(int stepperNb) {
    LOG_INFO("enable Stepper");
    if(i2c_smbus_write_byte(i2cFile, (stepperNb-1)*2 + 21)){
        LOG_ERROR("couldn't enable Stepper");
    }
    return 0; 
}

int Arduino::disableStepper(int stepperNb) {
    LOG_INFO("disable Stepper");
    if(i2c_smbus_write_byte(i2cFile,(stepperNb-1)*2 + 22)){
        LOG_ERROR("couldn't disable Stepper");
    }
    return 0; 
}

int Arduino::ledOn(int LedNb) {
    if(i2c_smbus_write_byte(i2cFile, (LedNb-1)*2 + 31)){
        LOG_ERROR("couldn't turn on the led");
    }
    return 0; 
}

int Arduino::ledOff(int LedNb) {
    if(i2c_smbus_write_byte(i2cFile, (LedNb-1)*2 + 32)){
        LOG_ERROR("couldn't turn off the led");
    }
    return 0; 
}

int Arduino::moveStepper(int absPosition, int stepperNb) {
    LOG_INFO("move Stepper");
    int length = 2;  // Nb of bytes to send
    uint8_t message[2];
    int values[] = {absPosition};

    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)(10+stepperNb), length, message)){
        LOG_ERROR("couldn't move Stepper");
    }
    return 0; 
}
