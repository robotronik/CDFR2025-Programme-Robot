#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "defs/structs.hpp"

bool returnToHome();
bool homeActuators();
void enableActuators();
void disableActuators();
bool isRobotInArrivalZone(position_t position);
void opponentInAction(position_t position);
void switchTeamSide(colorTeam_t color);
void setStockAsRemoved(int num);

// Basic functions (FSM)
bool takeStockPlatforms();
bool constructSingleTribune();

// Servo Control
bool movePlatformLifts(bool inside);
bool moveTribunePusher(bool outside);

// Stepper Control
bool movePlatformElevator(int level);
bool moveTribuneElevator(bool high);
bool moveLowColumnsRevolverAbs(int N);
bool moveHighColumnsRevolverAbs(int N);

// Input sensors
colorTeam_t readColorSensorSwitch();
bool readButtonSensor();
bool readLatchSensor();
bool readFrontColumnsSensors();

#endif // MYFUNCTION_H
