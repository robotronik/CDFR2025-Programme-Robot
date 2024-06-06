#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "config.h"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "tableState.hpp"
#include <math.h>


typedef enum {
    TAKEPLANT_INIT,
    TAKEPLANT_FORWARD,
    TAKEPLANT_REFORWARD,
    TAKEPLANT_BACKWARD,
    TAKEPLANT_TAKE,
    TAKEPLANT_END
} fsmtakePlant_t;

typedef enum {
    SOLARPANEL_INIT,
    SOLARPANEL_SETHOME,
    SOLARPANEL_FORWARD,
    SOLARPANEL_PUSHFOR,
    SOLARPANEL_BACKWARD,
    SOLARPANEL_PUSHBACK,
    SOLARPANEL_END
} fsmSolarPanel_t;

typedef enum {
    TESTPINCE_INIT,
    TESTPINCE_GOPLANT,
    TESTPINCE_TAKEPLANT,
    TESTPINCE_GOCORNE,
    TESTPINCE_GOJARDINIER,
    TESTPINCE_PLACE,
    TESTPINCE_GOBACKWARD,
    TESTPINCE_GOHOME
} fsmTestPince_t;

typedef enum {
    PUTPLANT_INIT,
    PUTPLANT_GOBORDER,
    PUTPLANT_PLACE,
} fsmTPutPlant_t;

typedef enum {
    INTIY_INIT,
    INTIY_BACKWARD
} fsminitY_t;


typedef enum {
    SETPOS_INIT,
    SETPOS_WAITINIT,
    SETPOS_FIRSTBACKWARD,
    SETPOS_FIRSTFORWARD,
    SETPOS_SECONDBACKWARD,
    SETPOS_SECONDFORWARD
} fsminitPos_t;


typedef enum {
    LASTPLANT_INIT,
    LASTPLANT_TAKE,
    LASTPLANT_GOPOT,
    LASTPLANT_REALSE,
    LASTPLANT_TURN
} fsminitLastPlant_t;


int initPositon2(tableState* itable, Asser* iAsser,int x, int y,int teta);
int turnSolarPannel(tableState* itable,Asser* iAsser,Arduino* arduino);
int takePlant(Asser* iAsser,Arduino* arduino,tableState*itable,int yPos,int xStart, int xEnd, int numPlante);
int takePlant2(Asser* iAsser,Arduino* arduino,tableState*itable,int xStart,int yStart, int xEnd, int yEnd);
int returnToHome(tableState* itable,Asser* iAsser);
int jardinierePutPlant(tableState* itable, Asser* iAsser,Arduino* arduino,int x,int y,int teta);
bool allJardiniereFull(tableState* itable);
void resetActionneur(Asser* iAsser, Arduino* arduino);
int FSMMatch(tableState* itable,Asser* iAsser,Arduino* arduino);
int TestPinceFSM(tableState* itable, Asser* iAsser,Arduino* arduino);
void ennemieInAction(tableState* itable, int x_ennemie, int y_ennemie );
#endif // MYFUNCTION_H
