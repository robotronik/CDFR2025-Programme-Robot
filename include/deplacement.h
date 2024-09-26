#pragma once

#include "cmdAsserv.hpp"
#include "structs.hpp"
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

int deplacementLinearPoint(int collide,CmdAsserv* robot, int x, int y);
int deplacementgoToPoint(int collide, CmdAsserv* robot, int x, int y, int teta, CmdAsserv::direction direction = CmdAsserv::MOVE_FORWARD,CmdAsserv::rotation rotationLookAt = CmdAsserv::ROTATION_DIRECT, CmdAsserv::rotation rotation = CmdAsserv::ROTATION_DIRECT);
int deplacementgoToPointNoTurn(int collide, CmdAsserv* robot, int x, int y, CmdAsserv::direction direction = CmdAsserv::MOVE_FORWARD,CmdAsserv::rotation rotationLookAt = CmdAsserv::ROTATION_DIRECT);