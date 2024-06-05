#include "actionContainer.hpp"

actionContainer::actionContainer( Asser* irobot, Arduino* iarduino, tableState* itable){
    robot = irobot;
    arduino = iarduino;
    table = itable;

    takePlante0 = new action("takePlante0",irobot,iarduino,itable);
    takePlante1 = new action("takePlante1",irobot,iarduino,itable);
    takePlante2 = new action("takePlante2",irobot,iarduino,itable);
    takePlante3 = new action("takePlante3",irobot,iarduino,itable);
    takePlante4 = new action("takePlante4",irobot,iarduino,itable);
    takePlante5 = new action("takePlante5",irobot,iarduino,itable);

    putInJardiniere0 = new action("putInJardiniere0",irobot,iarduino,itable);
    putInJardiniere1 = new action("putInJardiniere1",irobot,iarduino,itable);
    putInJardiniere2 = new action("putInJardiniere2",irobot,iarduino,itable);
    putInJardiniere3 = new action("putInJardiniere3",irobot,iarduino,itable);
    putInJardiniere4 = new action("putInJardiniere4",irobot,iarduino,itable);
    putInJardiniere5 = new action("putInJardiniere5",irobot,iarduino,itable);

    pushPotAction0 = new action("putInJardiniere5",irobot,iarduino,itable);
    pushPotAction1 = new action("putInJardiniere5",irobot,iarduino,itable);
    pushPotAction4 = new action("putInJardiniere5",irobot,iarduino,itable);
    pushPotAction5 = new action("putInJardiniere5",irobot,iarduino,itable);


    turnSolarPanelAction = new action("turnSolarPanelAction",irobot,iarduino,itable);

    returnToHomeAction = new action("returnToHomeAction",irobot,iarduino,itable);

    returnToHomeActionPlante = new action("returnToHomeAction",irobot,iarduino,itable);

}
void actionContainer::initAction( Asser* irobot, Arduino* iarduino, tableState* itable){
// ACTION
    takePlante0->setStartPoint(plantPosition[0].x - MARGESTOCKPLANT,plantPosition[0].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante0->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(iAsser,iarduino,itable,plantPosition[0].y,plantPosition[0].x - MARGESTOCKPLANT,plantPosition[0].x + 400,0);
    });
    takePlante0->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[0].etat = false;
    });
    takePlante0->setCostAction([](tableState*itable){
        return itable->planteStockFull[0].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ? -1 : -1;
    });
    listeAction.push_back(takePlante0);

// ACTION
    takePlante1->setStartPoint(plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante1->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY,plantPosition[1].x + MARGESTOCKPLANTX/5,plantPosition[1].y + MARGESTOCKPLANTY/5);
    });
    takePlante1->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[1].etat = false;
    });
    takePlante1->setCostAction([](tableState*itable){
        int cost = itable->robot.colorTeam == BLUE ? 90 : 70;
        return itable->planteStockFull[1].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante1);

// ACTION
    takePlante2->setStartPoint(plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante2->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY,plantPosition[2].x + MARGESTOCKPLANTX/5,plantPosition[2].y + MARGESTOCKPLANTY/5);
    });
    takePlante2->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[2].etat = false;
    });
    takePlante2->setCostAction([](tableState*itable){
        int cost = itable->robot.colorTeam == BLUE ? 100 : 80;
        return itable->planteStockFull[2].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante2);

// ACTION
    takePlante3->setStartPoint(plantPosition[3].x - MARGESTOCKPLANT,plantPosition[3].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante3->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(iAsser,iarduino,itable,plantPosition[3].y,plantPosition[3].x - MARGESTOCKPLANT,plantPosition[3].x + 400,3);
    });
    takePlante3->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[3].etat = false;
    });
    takePlante3->setCostAction([](tableState*itable){
        return itable->planteStockFull[3].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ? -1 : -1;
    });
    listeAction.push_back(takePlante3);

// ACTION
    takePlante4->setStartPoint(plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante4->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY,plantPosition[4].x + MARGESTOCKPLANTX/5,plantPosition[4].y - MARGESTOCKPLANTY/5);
    });
    takePlante4->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[4].etat = false;
    });
    takePlante4->setCostAction([](tableState*itable){
        int cost = itable->robot.colorTeam == BLUE ? 80 : 100;
        return itable->planteStockFull[4].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante4);

// ACTION
    takePlante5->setStartPoint(plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante5->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY,plantPosition[5].x + MARGESTOCKPLANTX/5,plantPosition[5].y - MARGESTOCKPLANTY/5);
    });
    takePlante5->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[5].etat = false;
    });
    takePlante5->setCostAction([](tableState*itable){
        int cost = itable->robot.colorTeam == BLUE ? 70 : 90;
        return itable->planteStockFull[5].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ? cost : -1;
    });
    listeAction.push_back(takePlante5);




     
// ACTION YELLOW
    putInJardiniere0->setStartPoint(JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere0->setEndPoint(JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere0->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable, iAsser, iarduino, JardinierePosition[0].x, JardinierePosition[0].y+130, 90);
    });
    putInJardiniere0->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[0].etat = true;
    });
    putInJardiniere0->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[0].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == YELLOW && itable->jardiniereFree[0].etat? 78 : -1;
    });
    listeAction.push_back(putInJardiniere0);


// ACTION BLUE
    putInJardiniere1->setStartPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setEndPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable, iAsser, iarduino, JardinierePosition[1].x, JardinierePosition[1].y+130, 90);
    });
    putInJardiniere1->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[1].etat = true;
    });
    putInJardiniere1->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[1].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == BLUE  && itable->jardiniereFree[1].etat? 89 : -1;
    });
    listeAction.push_back(putInJardiniere1);


// ACTION BLUE
    putInJardiniere2->setStartPoint(-700,-732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere2->setEndPoint(-700,-732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere2->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,-880,-732,-180);
    });
    putInJardiniere2->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[2].etat = true;
    });
    putInJardiniere2->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[2].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == BLUE ? 99 : -1;
    });
    listeAction.push_back(putInJardiniere2);


// ACTION YELLOW
    putInJardiniere3->setStartPoint(-700,732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,-880,732,-180);
    });
    putInJardiniere3->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[3].etat = true;
    });
    putInJardiniere3->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[3].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == YELLOW ? 99 : -1;
    });
    listeAction.push_back(putInJardiniere3);


// ACTION YELLOW
    putInJardiniere4->setStartPoint(JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setEndPoint(JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable, iAsser,iarduino,JardinierePosition[4].x, JardinierePosition[4].y-130,-90);
    });
    putInJardiniere4->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[4].etat = true;
    });
    putInJardiniere4->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[4].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == YELLOW  && itable->jardiniereFree[2].etat? 89 : -1;
    });
    listeAction.push_back(putInJardiniere4);

// ACTION BLUE
    putInJardiniere5->setStartPoint(JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere5->setEndPoint(JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere5->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,JardinierePosition[5].x , JardinierePosition[5].y-130, -90);
    });
    putInJardiniere5->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[5].etat = true;
    });
    putInJardiniere5->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[5].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == BLUE  && itable->jardiniereFree[3].etat? 78 : -1;
    });
    listeAction.push_back(putInJardiniere5);

    turnSolarPanelAction->setKeyMoment(65000);
    turnSolarPanelAction->setStartPoint(700,(itable->robot.colorTeam == YELLOW ? 1200 : -1200),(itable->robot.colorTeam == YELLOW ? 90 : -90), MOVE_FORWARD, ROTATION_DIRECT);
    turnSolarPanelAction->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return turnSolarPannel(itable, iAsser, iarduino);
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



    returnToHomeAction->setStartPoint(700,(itable->robot.colorTeam == YELLOW ? 1200 : -1200),(itable->robot.colorTeam == YELLOW ? -90 : 90), (itable->robot.colorTeam == BLUE ? MOVE_BACKWARD : MOVE_FORWARD), ROTATION_DIRECT);
    returnToHomeAction->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
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
    returnToHomeActionPlante->setStartPoint(plantPosition[0].x,plantPosition[0].y+250, -90, MOVE_FORWARD, ROTATION_DIRECT);
    returnToHomeActionPlante->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iret = 0;
        if(lastPlant(iAsser,iarduino,itable)){
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
    pushPotAction0->setStartPoint(JardinierePosition[0].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction0->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, JardinierePosition[0].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction0->goodEnd([](tableState*itable){
        itable->jardiniereFree[0].etat = true;
    });
    pushPotAction0->setCostAction([](tableState*itable){
        return itable->robot.colorTeam == YELLOW && !itable->jardiniereFree[0].etat ? -1 : -1;
    });
    listeAction.push_back(pushPotAction0);


//PUSH POT
    pushPotAction1->setStartPoint(JardinierePosition[1].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction1->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(itable->robot.collide, iAsser, JardinierePosition[1].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY+20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction1->goodEnd([](tableState*itable){
        itable->jardiniereFree[1].etat = true;
    });
    pushPotAction1->setCostAction([](tableState*itable){
        return itable->robot.colorTeam == BLUE && !itable->jardiniereFree[1].etat ? 98 : -1;
    });
    listeAction.push_back(pushPotAction1);


//PUSH POT
    pushPotAction4->setStartPoint(JardinierePosition[4].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction4->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(itable->robot.collide, iAsser, JardinierePosition[4].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY-20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction4->goodEnd([](tableState*itable){
        itable->jardiniereFree[2].etat = true;
    });
    pushPotAction4->setCostAction([](tableState*itable){
        return itable->robot.colorTeam == YELLOW && !itable->jardiniereFree[2].etat ? 98 : -1;
    });
    listeAction.push_back(pushPotAction4);


//PUSH POT
    pushPotAction5->setStartPoint(JardinierePosition[5].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction5->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, JardinierePosition[5].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction5->goodEnd([](tableState*itable){
        itable->jardiniereFree[3].etat = true;
    });
    pushPotAction5->setCostAction([](tableState*itable){
        return itable->robot.colorTeam == BLUE && !itable->jardiniereFree[3].etat ? -1 : -1;
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