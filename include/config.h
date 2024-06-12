#pragma once
#include "utils.h"
#include "constante.h"

typedef struct 
{
    int   x;
    int   y;
    int dist;
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

typedef struct
{
    colorTeam_t colorTeam;// équipe du robot
    position_t pos; //position du robot
    double vit_x,vit_y; //vitesse instantannée du robot
    int collide;
    //obstacle_t obstacle;

    bool robotHavePlante;// le robot a des plantes
    
}robot_t;
