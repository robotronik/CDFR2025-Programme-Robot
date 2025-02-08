#pragma once

#include "actions/action.hpp"
#include "actions/functions.h"
#include <vector>
#include "utils/logger.hpp"
#include "defs/constante.h"
#include "lidar/lidarAnalize.h"
#include "lidar/Lidar.hpp"

class actionContainer
{
private:
    Action* takeStock0;
    Action* takeStock1;
    Action* takeStock2;
    Action* takeStock3;
    Action* takeStock4;
    Action* takeStock5;
    Action* takeStock6;
    Action* takeStock7;
    Action* takeStock8;
    Action* takeStock9;

    Action* putInConstruction0;
    Action* putInConstruction1;
    Action* putInConstruction2;
    Action* putInConstruction3;
    Action* putInConstruction4;
    Action* putInConstruction5;

    Action * waitFin;
    Action* returnToHomeAction;
    Action* currentAction;

    std::vector<Action*> listeAction;

public:
    actionContainer();
    ~actionContainer();
    void init();
    void initAction();
    bool forceNextAction();
    int run();
    void resetAllAction();

private : 
    int choosNextAction();
};

