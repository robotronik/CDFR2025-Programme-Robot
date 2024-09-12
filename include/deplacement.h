#pragma once

#include "asser.hpp"
#include "config.h"
#include "arduino.hpp"
#include "constante.h"


typedef enum {
    DEPLACEMENT_INIT,
    DEPLACEMENT_WAITFIRSTMOVE,
    DEPLACEMENT_MOVE,
    DEPLACEMENT_STOP,
    DEPLACEMENT_WAIT
} deplcement_State_t;

typedef enum {
    GOTO_INIT,
    GOTO_LOOKAT,
    GOTO_MOVE,
    GOTO_TURN,
} go_to_State_t;

typedef enum {
    GOTO_INIT_PF,
    GOTO_CHOOSE_POINT_PF,
    GOTO_LOOKAT_PF,
    GOTO_MOVE_PF,
    GOTO_TURN_PF,
} go_to_State_PF_t;

int deplacementLinearPoint(int collide,Asser* robot, int x, int y);
int deplacementgoToPoint(int collide, Asser* robot, int x, int y, int teta, asser_direction_side direction = MOVE_FORWARD,asser_rotation_side rotationLookAt = ROTATION_DIRECT,asser_rotation_side rotation = ROTATION_DIRECT);
int deplacementgoToPointNoTurn(int collide, Asser* robot, int x, int y, asser_direction_side direction = MOVE_FORWARD,asser_rotation_side rotationLookAt = ROTATION_DIRECT);