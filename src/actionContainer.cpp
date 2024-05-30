#include "actionContainer.hpp"
#include "logger.hpp"

actionContainer::actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
    mainRobot = imainRobot;
    robot = irobot;
    arduino = iarduino;
    table = itable;

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

    pushPotAction0 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);
    pushPotAction1 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);
    pushPotAction4 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);
    pushPotAction5 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);


    turnSolarPanelAction = new action("turnSolarPanelAction",imainRobot,irobot,iarduino,itable);

    returnToHomeAction = new action("returnToHomeAction",imainRobot,irobot,iarduino,itable);

    returnToHomeActionPlante = new action("returnToHomeAction",imainRobot,irobot,iarduino,itable);

}
void actionContainer::initAction(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
// ACTION
    takePlante0->setStartPoint(itable->plantPosition[0].x - MARGESTOCKPLANT,itable->plantPosition[0].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante0->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[0].y,itable->plantPosition[0].x - MARGESTOCKPLANT,itable->plantPosition[0].x + 400,0);
    });
    takePlante0->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[0] = false;
    });
    takePlante0->setCostAction([](tableState*itable){
        return itable->planteStockFull[0] && !itable->robotHavePlante && !allJardiniereFull(itable) ? -1 : -1;
    });
    listeAction.push_back(takePlante0);

// ACTION
    takePlante1->setStartPoint(itable->plantPosition[1].x - MARGESTOCKPLANTX,itable->plantPosition[1].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante1->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[1].x - MARGESTOCKPLANTX,itable->plantPosition[1].y - MARGESTOCKPLANTY,itable->plantPosition[1].x + MARGESTOCKPLANTX/5,itable->plantPosition[1].y + MARGESTOCKPLANTY/5);
    });
    takePlante1->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[1] = false;
    });
    takePlante1->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 90 : 70;
        return itable->planteStockFull[1] && !itable->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante1);

// ACTION
    takePlante2->setStartPoint(itable->plantPosition[2].x - MARGESTOCKPLANTX,itable->plantPosition[2].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[2].x - MARGESTOCKPLANTX,itable->plantPosition[2].y - MARGESTOCKPLANTY,itable->plantPosition[2].x + MARGESTOCKPLANTX/5,itable->plantPosition[2].y + MARGESTOCKPLANTY/5);
    });
    takePlante2->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[2] = false;
    });
    takePlante2->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 100 : 80;
        return itable->planteStockFull[2] && !itable->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante2);

// ACTION
    takePlante3->setStartPoint(itable->plantPosition[3].x - MARGESTOCKPLANT,itable->plantPosition[3].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante3->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[3].y,itable->plantPosition[3].x - MARGESTOCKPLANT,itable->plantPosition[3].x + 400,3);
    });
    takePlante3->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[3] = false;
    });
    takePlante3->setCostAction([](tableState*itable){
        return itable->planteStockFull[3] && !itable->robotHavePlante && !allJardiniereFull(itable) ? -1 : -1;
    });
    listeAction.push_back(takePlante3);

// ACTION
    takePlante4->setStartPoint(itable->plantPosition[4].x - MARGESTOCKPLANTX,itable->plantPosition[4].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante4->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[4].x - MARGESTOCKPLANTX,itable->plantPosition[4].y + MARGESTOCKPLANTY,itable->plantPosition[4].x + MARGESTOCKPLANTX/5,itable->plantPosition[4].y - MARGESTOCKPLANTY/5);
    });
    takePlante4->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[4] = false;
    });
    takePlante4->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 80 : 100;
        return itable->planteStockFull[4] && !itable->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante4);

// ACTION
    takePlante5->setStartPoint(itable->plantPosition[5].x - MARGESTOCKPLANTX,itable->plantPosition[5].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante5->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(*iRobot,iAsser,iarduino,itable,itable->plantPosition[5].x - MARGESTOCKPLANTX,itable->plantPosition[5].y + MARGESTOCKPLANTY,itable->plantPosition[5].x + MARGESTOCKPLANTX/5,itable->plantPosition[5].y - MARGESTOCKPLANTY/5);
    });
    takePlante5->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[5] = false;
    });
    takePlante5->setCostAction([](tableState*itable){
        int cost = itable->colorTeam == BLUE ? 70 : 90;
        return itable->planteStockFull[5] && !itable->robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante5);




     
// ACTION YELLOW
    putInJardiniere0->setStartPoint(itable->JardinierePosition[0].x, itable->JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere0->setEndPoint(itable->JardinierePosition[0].x, itable->JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere0->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot, iAsser, iarduino, itable->JardinierePosition[0].x, itable->JardinierePosition[0].y+130, 90);
    });
    putInJardiniere0->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        itable->JardiniereFull[0] = true;
    });
    putInJardiniere0->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[0] && itable->robotHavePlante && itable->colorTeam == YELLOW && itable->jardiniereFree[0]? 78 : -1;
    });
    listeAction.push_back(putInJardiniere0);


// ACTION BLUE
    putInJardiniere1->setStartPoint(itable->JardinierePosition[1].x, itable->JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setEndPoint(itable->JardinierePosition[1].x, itable->JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot, iAsser, iarduino, itable->JardinierePosition[1].x, itable->JardinierePosition[1].y+130, 90);
    });
    putInJardiniere1->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        itable->JardiniereFull[1] = true;
    });
    putInJardiniere1->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[1] && itable->robotHavePlante && itable->colorTeam == BLUE  && itable->jardiniereFree[1]? 89 : -1;
    });
    listeAction.push_back(putInJardiniere1);


// ACTION BLUE
    putInJardiniere2->setStartPoint(-700,-732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere2->setEndPoint(-700,-732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,-880,-732,-180);
    });
    putInJardiniere2->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        itable->JardiniereFull[2] = true;
    });
    putInJardiniere2->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[2] && itable->robotHavePlante && itable->colorTeam == BLUE ? 99 : -1;
    });
    listeAction.push_back(putInJardiniere2);


// ACTION YELLOW
    putInJardiniere3->setStartPoint(-700,732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,-880,732,-180);
    });
    putInJardiniere3->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        itable->JardiniereFull[3] = true;
    });
    putInJardiniere3->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[3] && itable->robotHavePlante && itable->colorTeam == YELLOW ? 99 : -1;
    });
    listeAction.push_back(putInJardiniere3);


// ACTION YELLOW
    putInJardiniere4->setStartPoint(itable->JardinierePosition[4].x, itable->JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setEndPoint(itable->JardinierePosition[4].x, itable->JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,itable->JardinierePosition[4].x, itable->JardinierePosition[4].y-130,-90);
    });
    putInJardiniere4->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        itable->JardiniereFull[4] = true;
    });
    putInJardiniere4->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[4] && itable->robotHavePlante && itable->colorTeam == YELLOW  && itable->jardiniereFree[2]? 89 : -1;
    });
    listeAction.push_back(putInJardiniere4);

// ACTION BLUE
    putInJardiniere5->setStartPoint(itable->JardinierePosition[5].x, itable->JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere5->setEndPoint(itable->JardinierePosition[5].x, itable->JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere5->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,itable->JardinierePosition[5].x , itable->JardinierePosition[5].y-130, -90);
    });
    putInJardiniere5->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        itable->JardiniereFull[5] = true;
    });
    putInJardiniere5->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[5] && itable->robotHavePlante && itable->colorTeam == BLUE  && itable->jardiniereFree[3]? 78 : -1;
    });
    listeAction.push_back(putInJardiniere5);

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
    listeAction.push_back(turnSolarPanelAction);



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
    listeAction.push_back(returnToHomeActionPlante);


//PUSH POT
    pushPotAction0->setStartPoint(itable->JardinierePosition[0].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction0->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(*iRobot, iAsser, itable->JardinierePosition[0].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction0->goodEnd([](tableState*itable){
        itable->jardiniereFree[0] = true;
    });
    pushPotAction0->setCostAction([](tableState*itable){
        return itable->colorTeam == YELLOW && !itable->jardiniereFree[0] ? -1 : -1;
    });
    listeAction.push_back(pushPotAction0);


//PUSH POT
    pushPotAction1->setStartPoint(itable->JardinierePosition[1].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction1->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(*iRobot, iAsser, itable->JardinierePosition[1].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY+20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction1->goodEnd([](tableState*itable){
        itable->jardiniereFree[1] = true;
    });
    pushPotAction1->setCostAction([](tableState*itable){
        return itable->colorTeam == BLUE && !itable->jardiniereFree[1] ? 98 : -1;
    });
    listeAction.push_back(pushPotAction1);


//PUSH POT
    pushPotAction4->setStartPoint(itable->JardinierePosition[4].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction4->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(*iRobot, iAsser, itable->JardinierePosition[4].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY-20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction4->goodEnd([](tableState*itable){
        itable->jardiniereFree[2] = true;
    });
    pushPotAction4->setCostAction([](tableState*itable){
        return itable->colorTeam == YELLOW && !itable->jardiniereFree[2] ? 98 : -1;
    });
    listeAction.push_back(pushPotAction4);


//PUSH POT
    pushPotAction5->setStartPoint(itable->JardinierePosition[5].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction5->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(*iRobot, iAsser, itable->JardinierePosition[5].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction5->goodEnd([](tableState*itable){
        itable->jardiniereFree[3] = true;
    });
    pushPotAction5->setCostAction([](tableState*itable){
        return itable->colorTeam == BLUE && !itable->jardiniereFree[3] ? -1 : -1;
    });
    listeAction.push_back(pushPotAction5);


    //Choose first action
    choosNextAction();
}

int actionContainer::actionContainerRun(void){
    int iActionReturn;
    int iChoosNextReturn = 0;
    int iRet = 0;
    iActionReturn = currentAction->runAction();
    if(iActionReturn == -100){
        iRet = -100;
    }
    else if(iActionReturn!=0){
        resetActionneur(robot,arduino);
        iChoosNextReturn = choosNextAction();
    }
    // else if(forceNextAction()){
    //     resetActionneur(robot,arduino);
    //     iChoosNextReturn = choosNextAction();
    // }

    if(iChoosNextReturn == -1){
        iRet = -1;
    }
    return iRet;
}

actionContainer::~actionContainer(){
}

bool actionContainer::forceNextAction(void){
    bool bRet = false;
    for (action* elem : listeAction) {
        if(elem->actionNeedForce()){
            bRet = true;
        }
    }
    return bRet;
}

int actionContainer::choosNextAction(void){
    LOG_GREEN_INFO("CHOOSE NEW ACTION: ");
    int bestCost = -1;
    for (action* elem : listeAction) {
        int cost = elem->costAction();
        if(cost>bestCost){
            bestCost = cost;
            currentAction = elem;
        }
    }
    if(bestCost!=-1)LOG_GREEN_INFO("CHOOSE : ",currentAction->getName());
    return bestCost;
}

void actionContainer::resetAllAction(void){
    for (action* elem : listeAction) {
        elem->resetActionEnable();
    }
}