#pragma once
#include "config.h"

#define MARGESTOCKPLANT 300
#define MARGEJADINIERE 450

#define MARGESTOCKPLANTX 240
#define MARGESTOCKPLANTY 360

#define PUSHPOTMARGEY 220
#define PUSHPOTMARGEX1 -150
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

#define DISTANCESTOP   600
#define DISTANCERESTART 700

#define MAX_SPEED 10000
#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155
#define ROBOT_Y_OFFSET 60

const cartesian_position_t plantPosition[6] =
{{500,0},{300,-500},{-300,-500},{-500,0},{-300,500},{300,500}};
const cartesian_position_t JardinierePosition[6] =
{{387,-1500,YELLOW},{-387,-1500,BLUE},{-1000,-762,BLUE},{-1000,762,YELLOW},{-387,1500,YELLOW},{387,1500,BLUE}};
const cartesian_position_t zoneFull [6]=
{{-1275,-750,BLUE},{-1275,0,YELLOW},{-1275,750,BLUE},{1275,750,YELLOW},{1275,0,BLUE},{1275,-750,YELLOW}};
const cartesian_position_t JardiniereFree [4]=
{{387,-1500,YELLOW},{-387,-1500,BLUE},{-387,1500,YELLOW},{387,1500,BLUE}};
const int table_solar[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};
const int rayon[3] = {200,200,200}; //0 : rayon zones de plantes / 1 : rayon zone jardinière / 2 : rayon panneau solaire