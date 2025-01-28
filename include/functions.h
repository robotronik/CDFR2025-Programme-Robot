#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "structs.hpp"

int returnToHome();
void resetActionneur(); // Called when the robot is ready to move
void disableActionneur();
void opponentInAction(position_t* position);

//TODO : New functions (State machines)
int takeStock(int xStart,int yStart, int xEnd, int yEnd, int num_zone);
int construct(int x,int y,int theta);
bool takeStockPlatforms();
bool constructSingleTribune();

//TODO : New functions (Utils)
bool movePlatformLifts(bool inside);
bool movePlatformElevator(int level);
bool moveTribunePusher(bool outside);
bool moveTribuneElevator(bool high);

void blinkLed(int LED_ID,int periode);

colorTeam_t readColorSensorSwitch();

#endif // MYFUNCTION_H
