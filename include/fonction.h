#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "structs.hpp"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "tableState.hpp"
#include "lidarAnalize.h"
#include "lidar.h"
#include "constante.h"
#include <math.h>


//TODO : Remove
/*
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

*/

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


int initPosition2(TableState* itable, CmdAsserv* iAsser,int x, int y,int teta);
int returnToHome(TableState* itable,CmdAsserv* iAsser);
void resetActionneur(CmdAsserv* iAsser, Arduino* arduino);
void ennemieInAction(TableState* itable, position_t* position);

//TODO : Remove
/*
int turnSolarPannel(TableState* itable,CmdAsserv* iAsser,Arduino* arduino);
int takePlant2(CmdAsserv* iAsser,Arduino* arduino,TableState*itable,int xStart,int yStart, int xEnd, int yEnd, int num_zone);
int jardinierePutPlant(TableState* itable, CmdAsserv* iAsser,Arduino* arduino,int x,int y,int teta);
bool allJardiniereFull(TableState* itable);
bool DeuxJardiniereFull(TableState* itable);
bool allStockPlanteUsed(TableState* itable);
int VolPlante(TableState* itable, CmdAsserv* iAsser,Arduino* arduino,int x,int y,int teta);
*/


#endif // MYFUNCTION_H
