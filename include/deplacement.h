#pragma once

#include "commandesAsservissement.hpp"
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

int deplacementLinearPoint(int collide,commandesAsservissement* robot, int x, int y);
int deplacementgoToPoint(int collide, commandesAsservissement* robot, int x, int y, int teta, commandesAsservissement::direction direction = commandesAsservissement::MOVE_FORWARD,commandesAsservissement::rotation rotationLookAt = commandesAsservissement::ROTATION_DIRECT, commandesAsservissement::rotation rotation = commandesAsservissement::ROTATION_DIRECT);
int deplacementgoToPointNoTurn(int collide, commandesAsservissement* robot, int x, int y, commandesAsservissement::direction direction = commandesAsservissement::MOVE_FORWARD,commandesAsservissement::rotation rotationLookAt = commandesAsservissement::ROTATION_DIRECT);