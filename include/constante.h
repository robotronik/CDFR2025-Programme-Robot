#pragma once
#include "config.h"
#define SIZEDATALIDAR 15000
#define MARGESTOCKPLANT 300
#define MARGEJADINIERE 350

#define MARGESTOCKPLANTX 240
#define MARGESTOCKPLANTY 360

#define PUSHPOTMARGEY 200
#define PUSHPOTMARGEX1 -250
#define PUSHPOTMARGEX2 250

#define DELAYUPDOWN 1000
#define DELAYOPENCLOSE 500

#define ELEVATORUP 2200
#define ELEVATORPLANT 550

#define CLAMPCLOSE 0
#define CLAMPOPEN 180
#define CLAMPSLEEP 15
#define CLAMPSTOP 20

#define I2C_ASSER_ADDR 42
#define I2C_ARDUINO_ADDR 100
#define I2C_SSD1306_ADDR 16


#define DISTANCESTOP   400
#define DISTANCERESTART 500

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155
#define ROBOT_Y_OFFSET 50

const cartesian_position_t plantPosition[6] =
{{500,0},{300,-500},{-300,-500},{-500,0},{-300,500},{300,500}};
const cartesian_position_t JardinierePosition[6] =
{{387,-1500,YELLOW},{-387,-1500,BLUE},{-1000,-762,BLUE},{-1000,762,YELLOW},{-387,1500,YELLOW},{387,1500,BLUE}};
const cartesian_position_t zoneFull [6]=
{{-1275,-750,BLUE},{-1275,0,YELLOW},{-1275,750,BLUE},{1275,750,YELLOW},{1275,0,BLUE},{1275,-750,YELLOW}};
const int rayon[3] = {200,200,200};
const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};