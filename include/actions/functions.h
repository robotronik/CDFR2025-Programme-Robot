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
void switchStrategy(int strategy);
void setStockAsRemoved(int num);

// Basic functions (FSM)
bool takeStockPlatforms();
bool constructSingleTribune();
bool liftSingleTribune();

// Related to stock management
void getBestAvailableStock(); // TODO finish
int getBestStockPositionOff(int stockN, position_t fromPos);

// Servo Control
bool movePlatformLifts(int pos, bool slow = false);
bool moveTribunePusher(bool outside, bool slow = false);
bool moveServoFloorColumns(bool up);
bool moveClaws(int level);

// Stepper Control
bool movePlatformElevator(int level);
bool moveLowColumnsRevolverAbs(int N);
bool moveHighColumnsRevolverAbs(int N);

// DC Motor Control
bool moveTribuneElevator();
void stopTribuneElevator();

// Input sensors
colorTeam_t readColorSensorSwitch();
bool readButtonSensor();
bool readLatchSensor();
bool readLeftPusherSensor();
bool readRightPusherSensor();
bool readPusherSensors();

#endif // MYFUNCTION_H
