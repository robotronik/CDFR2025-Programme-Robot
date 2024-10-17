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
    action* takeStock0;
    action* takeStock1;
    action* takeStock2;
    action* takeStock3;
    action* takeStock4;
    action* takeStock5;
    action* takeStock6;
    action* takeStock7;
    action* takeStock8;
    action* takeStock9;

    action* putInConstruction0;
    action* putInConstruction1;
    action* putInConstruction2;
    action* putInConstruction3;
    action* putInConstruction4;
    action* putInConstruction5;

    action * waitFin;
    action* returnToHomeAction;
    action* currentAction;

    CmdAsserv* robot;
    Arduino* arduino;
    TableState* table;

    std::vector<action*> listeAction;

public:
    actionContainer( CmdAsserv* irobot, Arduino* iarduino, TableState* itable);
    void initAction( CmdAsserv* irobot, Arduino* iarduino, TableState* itable);
    bool forceNextAction(void);
    int actionContainerRun(CmdAsserv* robotI2C,TableState* itable);
    void resetAllAction(void);
    ~actionContainer();
private : 

    int choosNextAction(void);
};

