#pragma once

#include "action.hpp"
#include "fonction.h"
#include <vector>


#define MARGESTOCKPLANT 300
#define MARGEJADINIERE 450

#define MARGESTOCKPLANTX 240
#define MARGESTOCKPLANTY 360

#define PUSHPOTMARGEY 220
#define PUSHPOTMARGEX1 -150
#define PUSHPOTMARGEX2 250

class actionContainer
{
private:
    action* takePlante0;
    action* takePlante1;
    action* takePlante2;
    action* takePlante3;
    action* takePlante4;
    action* takePlante5;

    action* putInJardiniere0;
    action* putInJardiniere1;
    action* putInJardiniere2;
    action* putInJardiniere3;
    action* putInJardiniere4;
    action* putInJardiniere5;

    action* turnSolarPanelAction;
    action* returnToHomeAction;
    action* currentAction;
    action* returnToHomeActionPlante;

    action* pushPotAction0;
    action* pushPotAction1;
    action* pushPotAction4;
    action* pushPotAction5;

    robotCDFR* mainRobot;
    Asser* robot;
    Arduino* arduino;
    tableState* table;

    std::vector<action*> listeAction;

public:
    actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable);
    void initAction(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable);
    bool forceNextAction(void);
    int actionContainerRun(void);
    void resetAllAction(void);
    ~actionContainer();
private : 

    int choosNextAction(void);
};

