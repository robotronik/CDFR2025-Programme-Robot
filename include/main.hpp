#pragma once
#include "i2c/Asserv.hpp"
#include "i2c/Arduino.hpp"
#include "defs/tableState.hpp"
#include "defs/structs.hpp"
#include "defs/constante.h"
#include "lidar/Lidar.hpp"

typedef enum {
    INIT = 0,
    INITIALIZE = 1,
    WAITSTART = 3,
    RUN = 4,
    FIN = 5,
    STOP = 6,
    MANUAL = 7
} main_State_t;


//Extern means the variable is defined in main but accessible from other classes
extern main_State_t currentState;
extern main_State_t nextState;

extern TableState tableStatus;
extern Asserv robotI2C;
extern Arduino arduino;
extern Lidar lidar;

extern bool manual_ctrl;
// Declare a function pointer
extern bool (*manual_currentFunc)();