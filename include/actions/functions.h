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
bool deployBanner();

// Related to stock management
void getBestAvailableStock(); // TODO finish
int getBestStockPositionOff(int stockN, position_t fromPos);

// Servo Control
bool movePlatformLifts(int pos, int slow = 0);
bool moveTribunePusher(bool outside, bool slow = false);
bool moveClaws(int level);
bool moveBannerDeploy(bool outside);

// Stepper Control
bool movePlatformElevator(int level);
bool moveColumnsElevator(bool up);
bool moveLowColumnsRevolverAbs(int N);

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
