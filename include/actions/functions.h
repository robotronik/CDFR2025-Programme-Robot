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
bool constructSingleTribuneP(int planks);
bool liftSingleTribune();
bool lowerSingleTribune();
bool deployBannerFront();
bool deployBannerBack();
bool deployBanner(bool front);
bool liftAllColumns();
bool releaseAllColumns();

// Related to stock management
void getBestAvailableStock(); // TODO finish
int getBestStockPositionOff(int stockN, position_t fromPos);

// Servo Control
bool movePlatformLifts(int pos, bool slow = false);
bool moveTribunePusher(int pos, bool slow = false);
bool moveClaws(int level);
bool moveBannerDeploy(int position, bool front);
bool moveStringClaws(bool open);

// Stepper Control
bool movePlatformElevator(int level, int offset = 0);
bool moveColumnsElevator(bool up);
bool moveLowColumnsRevolverAbs(int N);
bool moveColumnsElevator(int level);

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
bool readLeftPlankSensor();
bool readRightPlankSensor();
bool readPlankSensors();

#endif // MYFUNCTION_H
