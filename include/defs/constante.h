#pragma once
#include "defs/structs.hpp"
#define SIZEDATALIDAR 15000

#define I2C_ASSER_ADDR 42
#define I2C_ARDUINO_ADDR 100

#define LOOP_TIME_MS 10

#define DISTANCESTOP   500

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define RETURN_HOME 10 //Points for coming back home action
#define MAX_SPEED 20000

// Define the nums of the arduino for the STEPPERS
#define PLATFORMS_ELEVATOR_STEPPER_NUM    1
#define TRIBUNES_ELEVATOR_STEPPER_NUM     2
#define COLOMNS_REVOLVER_LOW_STEPPER_NUM  3
#define COLOMNS_REVOLVER_HIGH_STEPPER_NUM 4
// Define the nums of the arduino for the SERVOS
#define TRIBUNES_PUSH_SERVO_NUM           1
#define PLATFORMS_LIFT_LEFT_SERVO_NUM     2
#define PLATFORMS_LIFT_RIGHT_SERVO_NUM    3
#define TRIBUNES_CLAWS_SERVO_NUM          4
#define BANNER_RELEASE_SERVO_NUM          5
// Define the nums of the arduino for the SENSORS
#define BUTTON_SENSOR_NUM                 1
#define LATCH_SENSOR_NUM                  2
#define COLOR_SENSOR_NUM                  3
#define FRONT_COLUMN_SENSOR1_NUM          4
#define FRONT_COLUMN_SENSOR2_NUM          5

#define SPEED_STOCK 8000 // Motor speed for collecting stock (mm/s)
#define STEPPER_SPEED 1000 // Stepper motor speed for collecting stock (steps/s)

const int rayon[3] = {200,200,250};
const position_t STOCK_POSITION_ARRAY[] = {{100, 100, 0}, {200, 200, 90}}; // TODO : Fill this in correctly
// Angle is either 0 for horizontal stock of 90 for vertical
const int STOCK_COUNT = 2; // and this