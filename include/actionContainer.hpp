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
    action* takePlante0droite;
    action* takePlante0droitebis;
    action* takePlante1droite;
    action* takePlante2droite;
    action* takePlante3droite;
    action* takePlante4droite;
    action* takePlante5droite;

    action* takePlante0gauche;
    action* takePlante0gaucheBis;
    action* takePlante1gauche;
    action* takePlante2gauche;
    action* takePlante3gauche;
    action* takePlante4gauche;
    action* takePlante5gauche;

    action* putInJardiniere0;
    action* putInJardiniere1;
    action* putInJardiniere2;
    action* putInJardiniere3;
    action* putInJardiniere4;
    action* putInJardiniere5;

    action * waitFin;
    action* turnSolarPanelAction;
    action* returnToHomeAction;
    action* currentAction;
    action* VolZone;
    action* VolJardiniereBlue;
    action* VolJardiniereYellow;

    action* pushPotAction0;
    action* pushPotAction1;
    action* pushPotAction4;
    action* pushPotAction5;

    Asser* robot;
    Arduino* arduino;
    TableState* table;

    std::vector<action*> listeAction;

public:
    actionContainer( Asser* irobot, Arduino* iarduino, TableState* itable);
    void initAction( Asser* irobot, Arduino* iarduino, TableState* itable);
    bool forceNextAction(void);
    int actionContainerRun(Asser* robotI2C,TableState* itable);
    void resetAllAction(void);
    ~actionContainer();
private : 

    int choosNextAction(void);
};

