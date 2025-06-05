#pragma once

#include "i2c/Asserv.hpp"
#include "defs/structs.hpp"
#include <utils/json.hpp>
using json = nlohmann::json;

#define NAV_MAX_STALL_TIME_MS 2000

// Navigation return type
typedef enum {
    NAV_IN_PROCESS,
    NAV_DONE,
    NAV_PAUSED, // In case the opponent is in front
    NAV_ERROR,  // If locked for too long, for example
} nav_return_t;

extern bool override_no_stop;

// Navigation functions
nav_return_t navigationGoTo(int x, int y, int theta, Direction direction = Direction::FORWARD,Rotation rotationLookAt = Rotation::SHORTEST, Rotation rotation = Rotation::SHORTEST, bool useHighways = false);
nav_return_t navigationGoToNoTurn(int x, int y, Direction direction = Direction::FORWARD,Rotation rotationLookAt = Rotation::SHORTEST, bool useHighways = false);
nav_return_t navigationPath(position_t path[], int pathLenght, Direction direction = Direction::SHORTEST, Rotation rotationLookAt = Rotation::SHORTEST, Rotation rotation = Rotation::SHORTEST);
nav_return_t navigationPathNoTurn(position_t path[], int pathLenght, Direction direction = Direction::SHORTEST, Rotation rotationLookAt = Rotation::SHORTEST);
void navigation_path_json(json& j);
void navigationOpponentDetection();
void fillCurrentPath(position_t path[], int pathLength);