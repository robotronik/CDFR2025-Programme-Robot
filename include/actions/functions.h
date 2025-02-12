#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "defs/structs.hpp"

int returnToHome();
void resetActionneur(); // Called when the robot is ready to move
void disableActionneur();
void opponentInAction(position_t position);
void switchTeamSide(colorTeam_t color);
void setStockAsRemoved(int num);

//New functions (FSM)
int takeStock(int xStart,int yStart, int xEnd, int yEnd, int num_zone);
int construct(int x,int y,int theta);
bool takeStockPlatforms();
bool constructSingleTribune();

// Servo Control
bool movePlatformLifts(bool inside);
bool moveTribunePusher(bool outside);

// Stepper Control
bool movePlatformElevator(int level);
bool moveTribuneElevator(bool high);

// Input sensors
colorTeam_t readColorSensorSwitch();
bool readButtonSensor();
bool readLatchSensor();
bool readFrontColumnsSensors();

#endif // MYFUNCTION_H
