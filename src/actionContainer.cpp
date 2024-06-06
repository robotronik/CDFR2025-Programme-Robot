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

// ACTION 1
    takePlante0->setStartPoint(plantPosition[0].x - MARGESTOCKPLANT,plantPosition[0].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante0->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(iAsser,iarduino,itable,plantPosition[0].y,plantPosition[0].x - MARGESTOCKPLANT,plantPosition[0].x + 400,0);
    });
    takePlante0->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[0].etat = false;
    });
    takePlante0->setCostAction(1,0,itable); //num action puis num zone de l'action
    
    listeAction.push_back(takePlante0);

// ACTION 1
    takePlante1->setStartPoint(plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante1->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY,plantPosition[1].x + MARGESTOCKPLANTX/5,plantPosition[1].y + MARGESTOCKPLANTY/5);
    });
    takePlante1->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[1].etat = false;
    });
    takePlante1->setCostAction(1,1,itable);
    listeAction.push_back(takePlante1);

// ACTION 1
    takePlante2->setStartPoint(plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante2->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY,plantPosition[2].x + MARGESTOCKPLANTX/5,plantPosition[2].y + MARGESTOCKPLANTY/5);
    });
    takePlante2->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[2].etat = false;
    });
    takePlante2->setCostAction(1,2,itable);
    listeAction.push_back(takePlante2);

// ACTION 1
    takePlante3->setStartPoint(plantPosition[3].x - MARGESTOCKPLANT,plantPosition[3].y,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante3->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(iAsser,iarduino,itable,plantPosition[3].y,plantPosition[3].x - MARGESTOCKPLANT,plantPosition[3].x + 400,3);
    });
    takePlante3->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[3].etat = false;
    });
    takePlante3->setCostAction(1,3,itable);
    listeAction.push_back(takePlante3);

// ACTION 1
    takePlante4->setStartPoint(plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante4->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY,plantPosition[4].x + MARGESTOCKPLANTX/5,plantPosition[4].y - MARGESTOCKPLANTY/5);
    });
    takePlante4->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[4].etat = false;
    });
    takePlante4->setCostAction(1,4,itable);
    listeAction.push_back(takePlante4);

// ACTION 1
    takePlante5->setStartPoint(plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante5->setRunAction([&](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY,plantPosition[5].x + MARGESTOCKPLANTX/5,plantPosition[5].y - MARGESTOCKPLANTY/5);
    });
    takePlante5->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[5].etat = false;
    });
    takePlante5->setCostAction(1,5,itable);
    listeAction.push_back(takePlante5);




     
// ACTION 2 YELLOW 
    putInJardiniere0->setStartPoint(JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere0->setEndPoint(JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere0->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable, iAsser, iarduino, JardinierePosition[0].x, JardinierePosition[0].y+130, 90);
    });
    putInJardiniere0->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[0].etat = true;
    });
    putInJardiniere0->setCostAction(2,0,itable);
    listeAction.push_back(putInJardiniere0);


// ACTION 2 BLUE
    putInJardiniere1->setStartPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setEndPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE, 90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere1->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable, iAsser, iarduino, JardinierePosition[1].x, JardinierePosition[1].y+130, 90);
    });
    putInJardiniere1->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[1].etat = true;
    });
    putInJardiniere1->setCostAction(2,1,itable);
    listeAction.push_back(putInJardiniere1);


// ACTION 2 BLUE
    putInJardiniere2->setStartPoint(-700,-732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere2->setEndPoint(-700,-732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere2->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,-880,-732,-180);
    });
    putInJardiniere2->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[2].etat = true;
    });
    putInJardiniere2->setCostAction(2,2,itable);
    listeAction.push_back(putInJardiniere2);


// ACTION 2 YELLOW
    putInJardiniere3->setStartPoint(-700,732,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,732,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,-880,732,-180);
    });
    putInJardiniere3->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[3].etat = true;
    });
    putInJardiniere3->setCostAction(2,3,itable);
    listeAction.push_back(putInJardiniere3);


// ACTION 2 YELLOW
    putInJardiniere4->setStartPoint(JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setEndPoint(JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere4->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable, iAsser,iarduino,JardinierePosition[4].x, JardinierePosition[4].y-130,-90);
    });
    putInJardiniere4->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[4].etat = true;
    });
    putInJardiniere4->setCostAction(2,4,itable);
    listeAction.push_back(putInJardiniere4);

// ACTION 2 BLUE
    putInJardiniere5->setStartPoint(JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_FORWARD, ROTATION_DIRECT);
    putInJardiniere5->setEndPoint(JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE, -90, MOVE_BACKWARD, ROTATION_DIRECT);
    putInJardiniere5->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,JardinierePosition[5].x , JardinierePosition[5].y-130, -90);
    });
    putInJardiniere5->goodEnd([](tableState*itable){
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[5].etat = true;
    });
    putInJardiniere5->setCostAction(2,5,itable);
    listeAction.push_back(putInJardiniere5);

// ACTION 3
    turnSolarPanelAction->setKeyMoment(65000);
    turnSolarPanelAction->setStartPoint(700,(itable->robot.colorTeam == YELLOW ? 1200 : -1200),(itable->robot.colorTeam == YELLOW ? 90 : -90), MOVE_FORWARD, ROTATION_DIRECT);
    turnSolarPanelAction->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return turnSolarPannel(itable, iAsser, iarduino);
    });
    turnSolarPanelAction->goodEnd([](tableState*itable){
        itable->solarPanelTurn.etat = true;
    });
    turnSolarPanelAction->setCostAction(3,0,itable);
    listeAction.push_back(turnSolarPanelAction);


// ACTION 4
    returnToHomeAction->setStartPoint(700,(itable->robot.colorTeam == YELLOW ? 1200 : -1200),(itable->robot.colorTeam == YELLOW ? -90 : 90), (itable->robot.colorTeam == BLUE ? MOVE_BACKWARD : MOVE_FORWARD), ROTATION_DIRECT);
    returnToHomeAction->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iret = 0;
        if(releasePlant(iarduino)){iret = -100;}
        return iret;});
    returnToHomeAction->setKeyMoment(85000);
    returnToHomeAction->goodEnd([](tableState*itable){});
    returnToHomeAction->setCostAction(4,0,itable);
    //listeAction.push_back(returnToHomeAction);

//ACTION 5
    //PLANT TO HOME
    returnToHomeActionPlante->setStartPoint(plantPosition[0].x,plantPosition[0].y+250, -90, MOVE_FORWARD, ROTATION_DIRECT);
    returnToHomeActionPlante->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iret = 0;
        if(lastPlant(iAsser,iarduino,itable)){iret = -100;}
        return iret;});
    returnToHomeAction->goodEnd([](tableState*itable){});
    returnToHomeActionPlante->setCostAction(5,0,itable);
    listeAction.push_back(returnToHomeActionPlante);


//PUSH POT ACTION 6
    pushPotAction0->setStartPoint(JardinierePosition[0].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction0->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, JardinierePosition[0].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction0->goodEnd([](tableState*itable){
        itable->jardiniereFree[0].etat = true;
    });
    pushPotAction0->setCostAction(6,0,itable);
    listeAction.push_back(pushPotAction0);


//PUSH POT ACTION 6
    pushPotAction1->setStartPoint(JardinierePosition[1].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction1->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(itable->robot.collide, iAsser, JardinierePosition[1].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY+20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction1->goodEnd([](tableState*itable){
        itable->jardiniereFree[1].etat = true;
    });
    pushPotAction1->setCostAction(6,1,itable);
    listeAction.push_back(pushPotAction1);


//PUSH POT ACTION 6
    pushPotAction4->setStartPoint(JardinierePosition[4].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction4->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPointNoTurn(itable->robot.collide, iAsser, JardinierePosition[4].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY-20, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction4->goodEnd([](tableState*itable){
        itable->jardiniereFree[2].etat = true;
    });
    pushPotAction4->setCostAction(6,2,itable);
    listeAction.push_back(pushPotAction4);


//PUSH POT ACTION 6
    pushPotAction5->setStartPoint(JardinierePosition[5].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY,-180, MOVE_BACKWARD, ROTATION_DIRECT);
    pushPotAction5->setRunAction([](action* iaction, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, JardinierePosition[5].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY, -180, MOVE_BACKWARD,ROTATION_DIRECT);
    });
    pushPotAction5->goodEnd([](tableState*itable){
        itable->jardiniereFree[3].etat = true;
    });
    pushPotAction5->setCostAction(6,3,itable);
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