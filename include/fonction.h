#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "config.h"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "tableState.hpp"
#include "lidarAnalize.h"
#include "lidar.h"
#include "constante.h"
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
    VITESSEE_INIT,
    SOLARPANEL_INIT,
    SOLARPANEL_FIN_INIT,
    SOLARPANEL_SETHOME,
    SOLARPANEL_FORWARD,
    SOLARPANEL_PUSHFOR,
    SOLARPANEL_BACKWARD,
    SOLARPANEL_PUSHBACK,
    SOLARPANEL_END
} fsmSolarPanel_t;

typedef enum {
    PUTPLANT_INIT,
    PUTPLANT_GOBORDER,
    PUTPLANT_PLACE,
} fsmTPutPlant_t;

typedef enum {
    VOLPLANT_INIT,
    VOLPLANT_GOBORDER,
    VOLPLANT_PLACE,
    VOLPLANT_BACKWARD,
    VOLPLANT_ZONEFIN,
    VOLPLANT_FIN,
} fsmVolPlant_t;

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


int initPosition2(tableState* itable, Asser* iAsser,int x, int y,int teta);
int turnSolarPannel(tableState* itable,Asser* iAsser,Arduino* arduino);
int takePlant2(Asser* iAsser,Arduino* arduino,tableState*itable,int xStart,int yStart, int xEnd, int yEnd, int num_zone);
int returnToHome(tableState* itable,Asser* iAsser);
int jardinierePutPlant(tableState* itable, Asser* iAsser,Arduino* arduino,int x,int y,int teta);
bool allJardiniereFull(tableState* itable);
bool DeuxJardiniereFull(tableState* itable);
bool allStockPlanteUsed(tableState* itable);
void resetActionneur(Asser* iAsser, Arduino* arduino);
void ennemieInAction(tableState* itable, position_t* position);
int VolPlante(tableState* itable, Asser* iAsser,Arduino* arduino,int x,int y,int teta);
#endif // MYFUNCTION_H
