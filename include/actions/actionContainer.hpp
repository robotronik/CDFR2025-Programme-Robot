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

    std::vector<action*> listeAction;

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

