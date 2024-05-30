#pragma once

#include <cstdint>
#include <iostream>
#include "I2CDevice.hpp"

#define I2C_ASSER_ADDR 42

typedef enum {
    MOVING_FINISHED = 0,
    RUNNING_FINISHED = 1,
    TURNING_FINISHED = 2
} status_finished;

typedef enum{
    MOVING_ERROR = 0,
    RUNNING_ERROR = 1,
    TURNING_ERROR = 2,
    LINEAR_ERROR = 3,
    ANGULAR_ERROR = 4
} asser_error_type;

typedef enum {
    ROTATION_DIRECT = 0,
    ROTATION_TRIGO = 1,
    ROTATION_HORRAIRE = 2
}asser_rotation_side;

typedef enum {
    MOVE_FORWARD = 0,
    MOVE_BACKWARD = 1,
}asser_direction_side;

// typedef enum{
//     TURN_CLOCKWISE = 
// }

class Asser : public I2CDevice {
    using I2CDevice::I2CDevice;
   
    public:
        Asser(int slave_address);
        int turnOnLed(int ledN);
        int turnOffLed(int ledN);

        int getCoords(int &x, int &y, int &theta);
        int setCoords(int x, int y, int z);
        int stop();
        int linearSetpoint(int x, int y);
        int angularSetpoint(int angle, int rotation);
        int setLookForward(int x, int y, int rotation);
        int setLookBackward(int x, int y, int rotation);
        bool getRobotFinished(status_finished status);
        int getError(asser_error_type error_type, int &error);
        int getError(asser_error_type error_type);
        int getBrakingDistance(int &distance);
        int enableMotor(bool status);
        int setLinearMaxSpeed(int maxSpeed);
        int setAngularMaxSpeed(int maxSpeed);
        int brakeMotor(bool brake);
        int setMaxTorque(int max);
};