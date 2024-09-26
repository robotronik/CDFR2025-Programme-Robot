#pragma once
#include "fonction.h"
#include "cmdAsserv.hpp"

typedef enum {
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    RUN,
    FIN,
    STOP
} main_State_t;


//Extern means the variable is defined in main
extern main_State_t currentState;
extern TableState tableStatus;
extern CmdAsserv *robotI2C;
extern lidarAnalize_t lidarData[SIZEDATALIDAR];

extern int countStart, count_pos;
extern int16_t x,y,teta,distance;
extern int countSetHome;