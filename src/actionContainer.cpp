#include "actionContainer.hpp"
#include "logger.hpp"

actionContainer::actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
    mainRobot = imainRobot;
    robot = irobot;
    arduino = iarduino;
    table = itable;

    listeAction = (Action_liste_t*)calloc( itable->dimensionZone,sizeof(Action_liste_t) );

    takePlante0 = new action("takePlante0",imainRobot,irobot,iarduino,itable);
    takePlante1 = new action("takePlante1",imainRobot,irobot,iarduino,itable);
    takePlante2 = new action("takePlante2",imainRobot,irobot,iarduino,itable);
    takePlante3 = new action("takePlante3",imainRobot,irobot,iarduino,itable);
    takePlante4 = new action("takePlante4",imainRobot,irobot,iarduino,itable);
    takePlante5 = new action("takePlante5",imainRobot,irobot,iarduino,itable);

    putInJardiniere0 = new action("putInJardiniere0",imainRobot,irobot,iarduino,itable);
    putInJardiniere1 = new action("putInJardiniere1",imainRobot,irobot,iarduino,itable);
    putInJardiniere2 = new action("putInJardiniere2",imainRobot,irobot,iarduino,itable);
    putInJardiniere3 = new action("putInJardiniere3",imainRobot,irobot,iarduino,itable);
    putInJardiniere4 = new action("putInJardiniere4",imainRobot,irobot,iarduino,itable);
    putInJardiniere5 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);

    pushPotAction0 = new action("putInJardiniere0",imainRobot,irobot,iarduino,itable);
    pushPotAction1 = new action("putInJardiniere1",imainRobot,irobot,iarduino,itable);
    pushPotAction4 = new action("putInJardiniere4",imainRobot,irobot,iarduino,itable);
    pushPotAction5 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);


    turnSolarPanelAction = new action("turnSolarPanelAction",imainRobot,irobot,iarduino,itable);

    returnToHomeAction = new action("returnToHomeAction",imainRobot,irobot,iarduino,itable);

    returnToHomeActionPlante = new action("returnToHomeAction",imainRobot,irobot,iarduino,itable);

}

void actionContainer::initAction(tableState* itable){
// ACTION
    takePlante0->setStartPointNoTeta(itable->plantPosition[0].x - MARGESTOCKPLANT,itable->plantPosition[0].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante0->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[0].y,itable->plantPosition[0].x - MARGESTOCKPLANT,itable->plantPosition[0].x + 400,0);
    });
    takePlante0->goodEnd([](tableState *itable){
        itable->nous->robotHavePlante = true;
        itable->Actions->takePlante0->setDone(true);
    });
    takePlante0->setCostAction([](tableState*itable){
        return !itable->Actions->takePlante0->isDone() && !itable->nous->robotHavePlante && !allJardiniereFull(itable) ? -1 : -1;
    });
    takePlante0->setZone(itable->dimensionZone);
    for (size_t i = 0; i < takePlante0->sizeof_action; i++)
    {
        listeAction[i].push_back(takePlante0);
    }
    
    

// ACTION
    takePlante1->setStartPointNoTeta(itable->plantPosition[1].x - MARGESTOCKPLANTX,itable->plantPosition[1].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante1->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[1].x - MARGESTOCKPLANTX,itable->plantPosition[1].y - MARGESTOCKPLANTY,itable->plantPosition[1].x + MARGESTOCKPLANTX/5,itable->plantPosition[1].y + MARGESTOCKPLANTY/5);
    });
    takePlante1->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = true;
        itable->Actions->takePlante1->setDone(true);
    });
    takePlante1->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 90 : 70;
        return !itable->Actions->takePlante1->isDone() && !itable->nous->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    takePlante1->setZone(itable->dimensionZone);
    for (size_t i = 0; i < takePlante1->sizeof_action; i++)
    {
        listeAction[i].push_back(takePlante1);
    }

// ACTION
    takePlante2->setStartPointNoTeta(itable->plantPosition[2].x - MARGESTOCKPLANTX,itable->plantPosition[2].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[2].x - MARGESTOCKPLANTX,itable->plantPosition[2].y - MARGESTOCKPLANTY,itable->plantPosition[2].x + MARGESTOCKPLANTX/5,itable->plantPosition[2].y + MARGESTOCKPLANTY/5);
    });
    takePlante2->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = true;
        itable->Actions->takePlante2->setDone(true);
    });
    takePlante2->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 100 : 80;
        return !itable->Actions->takePlante2->isDone() && !itable->nous->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    takePlante1->setZone(itable->dimensionZone);
    for (size_t i = 0; i < takePlante0->sizeof_action; i++)
    {
        listeAction[i].push_back(takePlante1);
    }

// ACTION
    takePlante3->setStartPointNoTeta(itable->plantPosition[3].x - MARGESTOCKPLANT,itable->plantPosition[3].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante3->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[3].y,itable->plantPosition[3].x - MARGESTOCKPLANT,itable->plantPosition[3].x + 400,3);
    });
    takePlante3->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = true;
        itable->Actions->takePlante3->setDone(true);
    });
    takePlante3->setCostAction([](tableState*itable){
        return !itable->Actions->takePlante3->isDone() && !itable->nous->robotHavePlante && !allJardiniereFull(itable) ? -1 : -1;
    });
    takePlante3->setZone(itable->dimensionZone);
    for (size_t i = 0; i < takePlante3->sizeof_action; i++)
    {
        listeAction[i].push_back(takePlante3);
    }

// ACTION
    takePlante4->setStartPointNoTeta(itable->plantPosition[4].x - MARGESTOCKPLANTX,itable->plantPosition[4].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante4->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[4].x - MARGESTOCKPLANTX,itable->plantPosition[4].y + MARGESTOCKPLANTY,itable->plantPosition[4].x + MARGESTOCKPLANTX/5,itable->plantPosition[4].y - MARGESTOCKPLANTY/5);
    });
    takePlante4->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = true;
        itable->Actions->takePlante4->setDone(true);
    });
    takePlante4->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 80 : 100;
        return !itable->Actions->takePlante4->isDone() && !itable->nous->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    takePlante4->setZone(itable->dimensionZone);
    for (size_t i = 0; i < takePlante4->sizeof_action; i++)
    {
        listeAction[i].push_back(takePlante4);
    }

// ACTION
    takePlante5->setStartPointNoTeta(itable->plantPosition[5].x - MARGESTOCKPLANTX,itable->plantPosition[5].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante5->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[5].x - MARGESTOCKPLANTX,itable->plantPosition[5].y + MARGESTOCKPLANTY,itable->plantPosition[5].x + MARGESTOCKPLANTX/5,itable->plantPosition[5].y - MARGESTOCKPLANTY/5);
    });
    takePlante5->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = true;
        itable->Actions->takePlante5->setDone(true);
    });
    takePlante5->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 70 : 90;
        return !itable->Actions->takePlante5->isDone() && !itable->nous->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    takePlante5->setZone(itable->dimensionZone);
    for (size_t i = 0; i < takePlante5->sizeof_action; i++)
    {
        listeAction[i].push_back(takePlante5);
    }




     
// ACTION YELLOW
    putInJardiniere0->setStartPoint(itable->JardinierePosition[0].x, itable->JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere0->setEndPoint(itable->JardinierePosition[0].x, itable->JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere0->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot, iAsser, iarduino, itable->JardinierePosition[0].x, itable->JardinierePosition[0].y+130, 90);
    });
    putInJardiniere0->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = false;
        itable->Actions->putInJardiniere0->setDone(true);
    });
    putInJardiniere0->setCostAction([](tableState*itable){
        return !itable->Actions->putInJardiniere0->isDone() && itable->nous->robotHavePlante && itable->colorTeam == YELLOW ? 78 : -1;
    });
    putInJardiniere0->setZone(itable->dimensionZone);
    for (size_t i = 0; i < putInJardiniere0->sizeof_action; i++)
    {
        listeAction[i].push_back(putInJardiniere0);
    }
    


// ACTION BLUE
    putInJardiniere1->setStartPoint(itable->JardinierePosition[1].x, itable->JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setEndPoint(itable->JardinierePosition[1].x, itable->JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot, iAsser, iarduino, itable->JardinierePosition[1].x, itable->JardinierePosition[1].y+130, 90);
    });
    putInJardiniere1->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = false;
        itable->Actions->putInJardiniere1->setDone(true);
    });
    putInJardiniere1->setCostAction([](tableState*itable){
        return itable->Actions->putInJardiniere1->isDone() && itable->nous->robotHavePlante && itable->colorTeam == BLUE  ? 89 : -1;
    });
    putInJardiniere1->setZone(itable->dimensionZone);
    for (size_t i = 0; i < putInJardiniere1->sizeof_action; i++)
    {
        listeAction[i].push_back(putInJardiniere1);
    }


// ACTION BLUE
    putInJardiniere2->setStartPoint(-700,-732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere2->setEndPoint(-700,-732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,-880,-732,-180);
    });
    putInJardiniere2->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = false;
        itable->Actions->putInJardiniere2->setDone(true);
    });
    putInJardiniere2->setCostAction([](tableState*itable){
        return !itable->Actions->putInJardiniere2->isDone() && itable->nous->robotHavePlante && itable->colorTeam == BLUE ? 99 : -1;
    });
    putInJardiniere2->setZone(itable->dimensionZone);
    for (size_t i = 0; i < putInJardiniere2->sizeof_action; i++)
    {
        listeAction[i].push_back(putInJardiniere2);
    }


// ACTION YELLOW
    putInJardiniere3->setStartPoint(-700,732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,-880,732,-180);
    });
    putInJardiniere3->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = false;
        itable->Actions->putInJardiniere3->setDone(true);
    });
    putInJardiniere3->setCostAction([](tableState*itable){
        return !itable->Actions->putInJardiniere3->isDone() && itable->nous->robotHavePlante && itable->colorTeam == YELLOW ? 99 : -1;
    });
    putInJardiniere3->setZone(itable->dimensionZone);
    for (size_t i = 0; i < putInJardiniere3->sizeof_action; i++)
    {
        listeAction[i].push_back(putInJardiniere3);
    }


// ACTION YELLOW
    putInJardiniere4->setStartPoint(itable->JardinierePosition[4].x, itable->JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setEndPoint(itable->JardinierePosition[4].x, itable->JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,itable->JardinierePosition[4].x, itable->JardinierePosition[4].y-130,-90);
    });
    putInJardiniere4->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = false;
        itable->Actions->putInJardiniere4->setDone(true);
    });
    putInJardiniere4->setCostAction([](tableState*itable){
        return !itable->Actions->putInJardiniere4->isDone() && itable->nous->robotHavePlante && itable->colorTeam == YELLOW  ? 89 : -1;
    });
    putInJardiniere4->setZone(itable->dimensionZone);
    for (size_t i = 0; i < putInJardiniere4->sizeof_action; i++)
    {
        listeAction[i].push_back(putInJardiniere4);
    }

// ACTION BLUE
    putInJardiniere5->setStartPoint(itable->JardinierePosition[5].x, itable->JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere5->setEndPoint(itable->JardinierePosition[5].x, itable->JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere5->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,itable->JardinierePosition[5].x , itable->JardinierePosition[5].y-130, -90);
    });
    putInJardiniere5->goodEnd([](tableState*itable){
        itable->nous->robotHavePlante = false;
        itable->Actions->putInJardiniere5->setDone(true);
    });
    putInJardiniere5->setCostAction([](tableState*itable){
        return !itable->Actions->putInJardiniere5->isDone() && itable->nous->robotHavePlante && itable->colorTeam == BLUE  ? 78 : -1;
    });
    putInJardiniere5->setZone(itable->dimensionZone);
    for (size_t i = 0; i < putInJardiniere5->sizeof_action; i++)
    {
        listeAction[i].push_back(putInJardiniere5);
    }

    turnSolarPanelAction->setKeyMoment(65000);
    turnSolarPanelAction->setStartPoint(700,(itable->colorTeam == YELLOW ? 1200 : -1200),(itable->colorTeam == YELLOW ? 90 : -90), MOVE_FORWARD, ROTATION_DIRECT);
    turnSolarPanelAction->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return turnSolarPannel(*iRobot, iAsser, iarduino);
    });
    turnSolarPanelAction->goodEnd([](tableState*itable){
        itable->solarPanelTurn = true;
    });
    turnSolarPanelAction->setCostAction([](tableState*itable){
        int icost;
        if(itable->startTime+60000 < millis()){
            icost = 200;
        }
        else{
            icost = 10;
        }
        return !(itable->solarPanelTurn) ? icost : -1;
    });
    turnSolarPanelAction->setZone(itable->dimensionZone);
    for (size_t i = 0; i < turnSolarPanelAction->sizeof_action; i++)
    {
        listeAction[i].push_back(turnSolarPanelAction);
    }



    returnToHomeAction->setStartPoint(700,(itable->colorTeam == YELLOW ? 1200 : -1200),(itable->colorTeam == YELLOW ? -90 : 90), (itable->colorTeam == BLUE ? MOVE_BACKWARD : MOVE_FORWARD), ROTATION_DIRECT);
    returnToHomeAction->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iret = 0;
        if(releasePlant(iarduino)){
            iret = -100;
        }
        return iret;
    });
    returnToHomeAction->setKeyMoment(85000);
    returnToHomeAction->goodEnd([](tableState*itable){

    });
    returnToHomeAction->setCostAction([](tableState*itable){
        int icost;
        if(itable->startTime+80000 < millis()){
            icost = 250;
        }
        else{
            icost = 9;
        }
        return icost;
    });
    //listeAction.push_back(returnToHomeAction);

    //PLANT TO HOME
    returnToHomeActionPlante->setStartPoint(itable->plantPosition[0].x,itable->plantPosition[0].y+250, -90, MOVE_FORWARD, ROTATION_DIRECT);
    returnToHomeActionPlante->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iret = 0;
        if(lastPlant(*iRobot,iAsser,iarduino,itable)){
            iret = -100;
        }
        return iret;
    });
    returnToHomeAction->goodEnd([](tableState*itable){

    });
    returnToHomeActionPlante->setCostAction([](tableState*itable){
        int icost;
        if(itable->startTime+80000 < millis()){
            icost = 250;
        }
        else{
            icost = 9;
        }
        return icost;
    });
    //listeAction.push_back(returnToHomeActionPlante);


//PUSH POT
    pushPotAction0->setStartPoint(itable->JardinierePosition[0].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction0->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(*iRobot, iAsser, itable->JardinierePosition[0].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction0->goodEnd([](tableState*itable){
        itable->Actions->pushPotAction0->setDone(true);
    });
    pushPotAction0->setCostAction([](tableState*itable){
        return itable->colorTeam == YELLOW && !itable->Actions->pushPotAction0->isDone() ? -1 : -1;
    });
    pushPotAction0->setZone(itable->dimensionZone);
    for (size_t i = 0; i < pushPotAction0->sizeof_action; i++)
    {
        listeAction[i].push_back(pushPotAction0);
    }


//PUSH POT
    pushPotAction1->setStartPointNoTeta(itable->JardinierePosition[1].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction1->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(*iRobot, iAsser, itable->JardinierePosition[1].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY+20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction1->goodEnd([](tableState*itable){
        itable->Actions->pushPotAction1->setDone(true);
    });
    pushPotAction1->setCostAction([](tableState*itable){
        return itable->colorTeam == BLUE && !itable->Actions->pushPotAction1->isDone() ? 98 : -1;
    });
    pushPotAction1->setZone(itable->dimensionZone);
    for (size_t i = 0; i < pushPotAction1->sizeof_action; i++)
    {
        listeAction[i].push_back(pushPotAction1);
    }

//PUSH POT
    pushPotAction4->setStartPointNoTeta(itable->JardinierePosition[4].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction4->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(*iRobot, iAsser, itable->JardinierePosition[4].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY-20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction4->goodEnd([](tableState*itable){
        itable->Actions->pushPotAction4->setDone(true);
    });
    pushPotAction4->setCostAction([](tableState*itable){
        return itable->colorTeam == YELLOW && !itable->Actions->pushPotAction4->isDone() ? 98 : -1;
    });
    pushPotAction4->setZone(itable->dimensionZone);
    for (size_t i = 0; i < pushPotAction4->sizeof_action; i++)
    {
        listeAction[i].push_back(pushPotAction4);
    }


//PUSH POT
    pushPotAction5->setStartPoint(itable->JardinierePosition[5].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction5->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(*iRobot, iAsser, itable->JardinierePosition[5].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction5->goodEnd([](tableState*itable){
        itable->Actions->pushPotAction5->setDone(true);
    });
    pushPotAction5->setCostAction([](tableState*itable){
        return itable->colorTeam == BLUE && !itable->Actions->pushPotAction5->isDone() ? -1 : -1;
    });
    pushPotAction5->setZone(itable->dimensionZone);
    for (size_t i = 0; i < pushPotAction5->sizeof_action; i++)
    {
        listeAction[i].push_back(pushPotAction5);
    }


    //Choose first action
    //choosNextAction();
}

// int actionContainer::actionContainerRun(void){
//     int iActionReturn;
//     int iChoosNextReturn = 0;
//     int iRet = 0;
//     iActionReturn = currentAction->runAction();
//     if(iActionReturn == -100){
//         iRet = -100;
//     }
//     else if(iActionReturn!=0){
//         resetActionneur(robot,arduino);
//         iChoosNextReturn = choosNextAction();
//     }
//     // else if(forceNextAction()){
//     //     resetActionneur(robot,arduino);
//     //     iChoosNextReturn = choosNextAction();
//     // }

//     if(iChoosNextReturn == -1){
//         iRet = -1;
//     }
//     return iRet;
// }

actionContainer::~actionContainer(){
}

// bool actionContainer::forceNextAction(void){
//     bool bRet = false;
//     for (action* elem : listeAction) {
//         if(elem->actionNeedForce()){
//             bRet = true;
//         }
//     }
//     return bRet;
// }

// int actionContainer::choosNextAction(void){
//     LOG_GREEN_INFO("CHOOSE NEW ACTION: ");
//     int bestCost = -1;
//     for (action* elem : listeAction) {
//         int cost = elem->costAction();
//         if(cost>bestCost){
//             bestCost = cost;
//             currentAction = elem;
//         }
//     }
//     if(bestCost!=-1)LOG_GREEN_INFO("CHOOSE : ",currentAction->getName());
//     return bestCost;
// }

// void actionContainer::resetAllAction(void){
//     for (action* elem : listeAction) {
//         elem->resetActionEnable();
//     }
// }