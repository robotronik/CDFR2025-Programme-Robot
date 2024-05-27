#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <filesystem>
#include <pigpio.h>
#include <thread>
#include <fstream>

#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "asser.hpp"
#include "arduino.hpp"
#include "utils.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "robot.h"
#include "actionContainer.hpp"

#define DISABLE_LIDAR
#define SIZEDATALIDAR 10000

robotCDFR mainRobot;
Asser *robotI2C = new Asser(I2C_ASSER_ADDR);
//LOG_SETROBOT(robotI2C);
lidarAnalize_t lidarData[SIZEDATALIDAR];    
Arduino *arduino = new Arduino(100);
main_State_t currentState = INIT;
main_State_t nextState = INIT;
bool initStat = true;
actionContainer* Actions = new actionContainer(&mainRobot, robotI2C, arduino, &(mainRobot.tableStatus));
int countStart = 0;
int countSetHome = 0;