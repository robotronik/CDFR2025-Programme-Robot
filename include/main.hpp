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


//Extern means the variable is defined in main but accessible from other classes
extern main_State_t currentState;
extern bool initState
extern TableState tableStatus;
extern CmdAsserv *robotI2C;
extern lidarAnalize_t lidarData[SIZEDATALIDAR];
extern int lidar_count;