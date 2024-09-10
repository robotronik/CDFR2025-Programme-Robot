#pragma once
#include "fonction.h"

typedef enum {
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    RUN,
    FIN,
    STOP
} main_State_t;

static main_State_t currentState;
static TableState tableStatus;

static int countStart = 0, x=0, y=0, teta=0, count_pos = 0;
static int distance, countSetHome = 0;