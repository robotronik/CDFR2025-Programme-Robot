#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "structs.hpp"
#include "navigation.h"
#include "tableState.hpp"
#include "lidarAnalize.h"
#include "lidar.h"
#include "constante.h"
#include <math.h>

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


int initPosition2(TableState* itable, CmdAsserv* iAsser,int x, int y,int theta);
int returnToHome(TableState* itable,CmdAsserv* iAsser);
void resetActionneur(CmdAsserv* iAsser, Arduino* arduino);
void opponentInAction(TableState* itable, position_t* position);

//TODO : New functions
int takeStock(CmdAsserv* iAsser,Arduino* arduino,TableState*itable,int xStart,int yStart, int xEnd, int yEnd, int num_zone);
int construct(TableState* itable, CmdAsserv* iAsser,Arduino* arduino,int x,int y,int theta);

void blinkLed(Arduino* arduino,int LedNb,int periode);

#endif // MYFUNCTION_H
