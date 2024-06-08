#pragma once 
#include "constante.h"
#include "asser.hpp"
#include "config.h"
#include "arduino.hpp"


int pullpush(Arduino* arduino);
int catchPlant(Arduino* arduino);
int releasePlant(Arduino* arduino);
void blinkLed(Arduino* arduino,int LedNb,int periode);

