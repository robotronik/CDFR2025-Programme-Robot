#pragma once

#include "cmdAsserv.hpp"
#include "structs.hpp"
#include "arduino.hpp"
#include "tableState.hpp"
#include "constante.h"

#define NAV_MAX_STALL_TIME_MS 5000

typedef enum {
    NAV_INIT,
    NAV_WAITFIRSTMOVE,
    NAV_MOVE,
    NAV_STOP,
    NAV_WAIT
} deplcement_State_t;

typedef enum {
    GOTO_INIT,
    GOTO_LOOKAT,
    GOTO_MOVE,
    GOTO_TURN,
} go_to_State_t;

// Navigation return type
typedef enum {
    NAV_IN_PROCESS,
    NAV_DONE,
    NAV_PAUSED, // In case the opponent is in front
    NAV_ERROR,  // If locked for too long, for example
} nav_return_t;

void initNavigation(CmdAsserv* robot, TableState* table);
nav_return_t navigationGoTo(int x, int y, int theta, CmdAsserv::Direction direction = CmdAsserv::FORWARD,CmdAsserv::Rotation rotationLookAt = CmdAsserv::SHORTEST, CmdAsserv::Rotation rotation = CmdAsserv::SHORTEST);
nav_return_t navigationGoToNoTurn(int x, int y, CmdAsserv::Direction direction = CmdAsserv::FORWARD,CmdAsserv::Rotation rotationLookAt = CmdAsserv::SHORTEST);
void navigationOpponentDetection();