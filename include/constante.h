#pragma once
#include "structs.hpp"
#define SIZEDATALIDAR 15000

//#define DELAYUPDOWN 1000 //TODO
//#define DELAYOPENCLOSE 500

//#define ELEVATORUP 4000

//#define CLAMPCLOSE 0
//#define CLAMPOPEN 180
//#define CLAMPSLEEP 15
//#define CLAMPSTOP 20

#define I2C_ASSER_ADDR 42
#define I2C_ARDUINO_ADDR 100
#define I2C_SSD1306_ADDR 16


#define DISTANCESTOP   500
#define DISTANCERESTART 600

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155 // TODO
#define ROBOT_Y_OFFSET 130

#define RETURN_HOME 10 //Points for coming back home action
#define MAX_SPEED 20000
#define SPEED_STOCK 8000

const int rayon[3] = {200,200,250};
const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};