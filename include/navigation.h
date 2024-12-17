#pragma once

#include "cmdAsserv.hpp"
#include "structs.hpp"
#include "arduino.hpp"
#include "tableState.hpp"
#include "constante.h"

#define NAV_MAX_STALL_TIME_MS 5000

// Navigation return type
typedef enum {
    NAV_IN_PROCESS,
    NAV_DONE,
    NAV_PAUSED, // In case the opponent is in front
    NAV_ERROR,  // If locked for too long, for example
} nav_return_t;

void initNavigation(CmdAsserv* robot, TableState* table);
nav_return_t navigationGoTo(int x, int y, int theta, Direction direction = Direction::FORWARD,Rotation rotationLookAt = Rotation::SHORTEST, Rotation rotation = Rotation::SHORTEST);
nav_return_t navigationGoToNoTurn(int x, int y, Direction direction = Direction::FORWARD,Rotation rotationLookAt = Rotation::SHORTEST);
void navigationOpponentDetection();