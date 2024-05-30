#pragma once


#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "robot.h"
#include "fonction.h"
#include "action.hpp"



typedef enum {
    MATCH_INIT,
    MATCH_PLANT,
    MATCH_GOSOLARPANEL,
    MATCH_INTISOLARPANEL,
    MATCH_SOLARPANEL,
    MATCH_GOHOME1,
    MATCH_GOHOME2,
    MATCH_END
} fsmMatch_t;