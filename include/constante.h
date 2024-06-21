#pragma once
#include "config.h"
#define SIZEDATALIDAR 15000
#define MARGESTOCKPLANT 300
#define MARGEJADINIERE 200

#define MARGESTOCKPLANTX 200
#define MARGESTOCKPLANTY 300
#define DIVIDE 6

#define PUSHPOTMARGEY 200
#define PUSHPOTMARGEX1 -250
#define PUSHPOTMARGEX2 250

#define DELAYUPDOWN 1000
#define DELAYOPENCLOSE 500

#define ELEVATORUP 4000
#define ELEVATORPLANT 600
#define ELEVATORJARDINIERE 6000

#define CLAMPCLOSE 0
#define CLAMPOPEN 180
#define CLAMPSLEEP 15
#define CLAMPSTOP 20

#define I2C_ASSER_ADDR 42
#define I2C_ARDUINO_ADDR 100
#define I2C_SSD1306_ADDR 16


#define DISTANCESTOP   500
#define DISTANCERESTART 600

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155
#define ROBOT_Y_OFFSET 130

#define PLANTE_JARDINIERE 20
#define SOLAR_PANNEL 5
#define RETURN_HOME 10
#define MAX_SPEED 20000
#define SPEED_PLANTE 8000

const cartesian_position_t plantPosition[6] =
{{500,0},{300,-500},{-300,-500},{-500,0},{-300,500},{300,500}};
const cartesian_position_t JardinierePosition[6] =
{{387,-1500,YELLOW},{-387,-1500,BLUE},{-1000,-762,BLUE},{-1000,762,YELLOW},{-387,1500,YELLOW},{387,1500,BLUE}};
const cartesian_position_t zoneFull [6]=
{{-1275,-750,BLUE},{-1275,0,YELLOW},{-1275,750,BLUE},{1275,750,YELLOW},{1275,0,BLUE},{1275,-750,YELLOW}};
const int rayon[3] = {200,200,250};
const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};


/*temps actions min
push pot debut : 2:20
take plante 1 : 8:20
put in jardiniere : 7:30
take plant 2 : 10:09
put in jardiniere 2 : 7:40
solar panel : 21:24
take plant 3 : 6:83
put in jardiniere 3 : 12:39
vol plante : 12:19

tot = 1m28.18
*/