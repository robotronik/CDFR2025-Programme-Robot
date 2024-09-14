#pragma once
#include "fonction.h"
#include "asser.hpp"

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
extern Asser *robotI2C;
extern lidarAnalize_t lidarData[SIZEDATALIDAR];

extern int countStart, x, y, teta, count_pos;
extern int distance, countSetHome;