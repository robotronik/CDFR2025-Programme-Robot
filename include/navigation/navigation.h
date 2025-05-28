#pragma once

#include "i2c/Asserv.hpp"
#include <utils/json.hpp>
using json = nlohmann::json;

#define NAV_MAX_STALL_TIME_MS 5000

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
void navigation_path_json(json& j);
void navigationOpponentDetection();