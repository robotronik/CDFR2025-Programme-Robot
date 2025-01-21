#pragma once
#include "cmdAsserv.hpp"
#include "arduino.hpp"
#include "tableState.hpp"
#include "structs.hpp"
#include "constante.h"

typedef enum {
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    RUN,
    FIN,
    STOP,
    MANUAL
} main_State_t;


//Extern means the variable is defined in main but accessible from other classes
extern main_State_t currentState;
extern main_State_t nextState;

extern TableState tableStatus;
extern CmdAsserv robotI2C;
extern Arduino arduino;

extern lidarAnalize_t lidarData[SIZEDATALIDAR];
extern int lidar_count;

extern bool manual_ctrl;
// Declare a function pointer
extern bool (*manual_currentFunc)();