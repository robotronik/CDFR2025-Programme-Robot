#pragma once
#include "structs.hpp"
#define SIZEDATALIDAR 15000

#define I2C_ASSER_ADDR 42
#define I2C_ARDUINO_ADDR 100
#define I2C_SSD1306_ADDR 16 //TODO Remove

#define LOOP_TIME_MS 10

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
const position_t STOCK_POSITION_ARRAY = {(100, 100, 0), (200, 200, 90)}; // TODO : Fill this in correctly
// Angle is either 0 for horizontal stock of 90 for vertical
const int STOCK_COUNT = 2; // and this