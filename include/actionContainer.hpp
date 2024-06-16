#pragma once

#include "action.hpp"
#include "fonction.h"
#include <vector>
#include "logger.hpp"
#include "constante.h"
#include "lidarAnalize.h"
#include "lidar.h"

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

    action* pushPotAction0;
    action* pushPotAction1;
    action* pushPotAction4;
    action* pushPotAction5;

    Asser* robot;
    Arduino* arduino;
    tableState* table;

    std::vector<action*> listeAction;

public:
    actionContainer( Asser* irobot, Arduino* iarduino, tableState* itable);
    void initAction( Asser* irobot, Arduino* iarduino, tableState* itable);
    bool forceNextAction(void);
    int actionContainerRun(Asser* robotI2C,tableState* itable);
    void resetAllAction(void);
    ~actionContainer();
private : 

    int choosNextAction(void);
};

