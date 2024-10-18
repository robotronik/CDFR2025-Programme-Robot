#pragma once 
#include "constante.h"
#include "cmdAsserv.hpp"
#include "structs.hpp"
#include "arduino.hpp"


int pullpush(Arduino* arduino);

//TODO : Remove
//int catchPlant(Arduino* arduino);
//int catchPlant2(Arduino* arduino);
//int FastReleasePlant(Arduino* arduino);
//int releasePlant(Arduino* arduino);
void blinkLed(Arduino* arduino,int LedNb,int periode);

