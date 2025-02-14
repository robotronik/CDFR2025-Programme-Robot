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

#define COL_OFFSET_STOCK 320
#define NO_COL_OFFSET_STOCK 165 

// Angle is either 0 for horizontal stock of 90 for vertical
const position_t STOCK_POSITION_ARRAY[] = {{-725, 675, 0}, {-325, 1425, 90}, {600, 1425, 90}, {750, 725, 0}, {50, 400, 0}, 
                                           {-725,-675, 0}, {-325,-1425, 90}, {600,-1425, 90}, {750,-725, 0}, {50,-400, 0}};

// Offset of the robot from the stocks
const position_t STOCK_OFFSETS[] = {
    { COL_OFFSET_STOCK,-NO_COL_OFFSET_STOCK,-90},  //0 : Offset for horizontal droite bas
    { COL_OFFSET_STOCK, NO_COL_OFFSET_STOCK, 90},  //1 : Offset for horizontal droite haut
    {-COL_OFFSET_STOCK,-NO_COL_OFFSET_STOCK,-90},  //2 : Offset for horizontal gauche bas
    {-COL_OFFSET_STOCK, NO_COL_OFFSET_STOCK, 90},  //3 : Offset for horizontal gauche haut

    { NO_COL_OFFSET_STOCK,-COL_OFFSET_STOCK, 0},   //4 : Offset for vertical droite bas
    { NO_COL_OFFSET_STOCK, COL_OFFSET_STOCK, 0},   //5 : Offset for vertical droite haut
    {-NO_COL_OFFSET_STOCK,-COL_OFFSET_STOCK, 180}, //6 : Offset for vertical gauche bas
    {-NO_COL_OFFSET_STOCK, COL_OFFSET_STOCK, 180}, //7 : Offset for vertical gauche haut
}; 
const int STOCK_OFFSET_MAPPING[10][4] = {
    {0, -1, -1,-1}, // Stock 0 utilise les offsets 0
    {6, 7, -1,-1},  // Stock 1 utilise les offsets 6 et 7
    {6, 7, -1,-1},  // Stock 2 utilise les offsets 6 et 7
    {1, 3, -1,-1},  // Stock 3 utilise les offsets 1 et 3
    {0, 1, 2, 3},   // Stock 4 utilise les offsets 0,1,2 et 3

    {2, -1, -1,-1}, // Stock 5 utilise les offsets 2
    {4, 5, -1,-1},  // Stock 6 utilise les offsets 4 et 5
    {4, 5, -1,-1},  // Stock 7 utilise les offsets 4 et 5    
    {1, 3, -1,-1},  // Stock 8 utilise les offsets 1 et 3
    {0, 1, 2, 3},   // Stock 9 utilise les offsets 0,1,2 et 4
};

const int STOCK_COUNT = 10;
const int PROTECTED_BLUE_STOCK = 0;
const int PROTECTED_YELLOW_STOCK = 5;