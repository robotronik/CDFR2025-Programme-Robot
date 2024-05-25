#pragma once

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "logger.hpp"
#include "robot.h"

#define DISTANCESTOP   600
#define DISTANCERESTART 700

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

int deplacementLinearPoint(robotCDFR mainRobot,Asser* robot, int x, int y);
int deplacementgoToPoint(robotCDFR mainRobot, Asser* robot, int x, int y, int teta, asser_direction_side direction = MOVE_FORWARD,asser_rotation_side rotationLookAt = ROTATION_DIRECT,asser_rotation_side rotation = ROTATION_DIRECT);
int deplacementgoToPointNoTurn(robotCDFR mainRobot, Asser* robot, int x, int y, asser_direction_side direction = MOVE_FORWARD,asser_rotation_side rotationLookAt = ROTATION_DIRECT);