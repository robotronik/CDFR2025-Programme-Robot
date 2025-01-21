#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "structs.hpp"

typedef enum {
    INTIY_INIT,
    INTIY_BACKWARD,
} fsminitY_t;

typedef enum {
    SETPOS_INIT,
    SETPOS_WAITINIT,
    SETPOS_FIRSTBACKWARD,
    SETPOS_FIRSTFORWARD,
    SETPOS_SECONDBACKWARD,
    SETPOS_SECONDFORWARD
} fsminitPos_t;


int initPosition2(int x, int y,int theta);
int returnToHome();
void resetActionneur(); // Called at the boot and stop
void enableActionneur(); // Called when the robot is ready to move
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

void blinkLed(int LedNb,int periode);

#endif // MYFUNCTION_H
