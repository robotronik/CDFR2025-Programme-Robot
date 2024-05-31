#pragma once
#include "utils.h"
#include "constante.h"

typedef struct 
{
    int   x;
    int   y;
    int   teta;
    int   time;
}position_t;

typedef struct 
{
    bool   valid;
    double   angle;
    double   dist;
    int   x;
    int   y;
    bool   onTable;
}lidarAnalize_t;