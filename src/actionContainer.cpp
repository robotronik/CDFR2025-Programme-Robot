#include "actionContainer.hpp"

actionContainer::actionContainer(){}
void actionContainer::init(CmdAsserv* irobot, Arduino* iarduino, TableState* itable){
    robot = irobot;
    arduino = iarduino;
    table = itable;

    takeStock0 = new action("takeStock0",irobot,iarduino,itable);
    takeStock1 = new action("takeStock1",irobot,iarduino,itable);
    takeStock2 = new action("takeStock2",irobot,iarduino,itable);
    takeStock3 = new action("takeStock3",irobot,iarduino,itable);
    takeStock4 = new action("takeStock4",irobot,iarduino,itable);
    takeStock5 = new action("takeStock5",irobot,iarduino,itable);
    takeStock6 = new action("takeStock6",irobot,iarduino,itable);
    takeStock7 = new action("takeStock7",irobot,iarduino,itable);
    takeStock8 = new action("takeStock8",irobot,iarduino,itable);
    takeStock9 = new action("takeStock9",irobot,iarduino,itable);

    putInConstruction0 = new action("putInConstruction0",irobot,iarduino,itable);
    putInConstruction1 = new action("putInConstruction1",irobot,iarduino,itable);
    putInConstruction2 = new action("putInConstruction2",irobot,iarduino,itable);
    putInConstruction3 = new action("putInConstruction3",irobot,iarduino,itable);
    putInConstruction4 = new action("putInConstruction4",irobot,iarduino,itable);
    putInConstruction5 = new action("putInConstruction5",irobot,iarduino,itable);

    waitFin = new action("waitFin",irobot,iarduino,itable);
    returnToHomeAction = new action("returnToHomeAction",irobot,iarduino,itable);

}

void actionContainer::initAction( CmdAsserv* irobot, Arduino* iarduino, TableState* itable){
    
// ACTION 1 : CHERCHER DU STOCK
    takeStock0->setStartPoint(0,0,Direction::FORWARD,Rotation::SHORTEST); //replace 0,0 by coords of Stock[0]
    takeStock0->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        //return takePlant2(iAsser,iarduino,itable,plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY,plantPosition[0].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[0].y + MARGESTOCKPLANTY/DIVIDE,0);
        return takeStock(0,0,0,0,0);
    });
    takeStock0->goodEnd([](TableState*itable,CmdAsserv*irobot){
        //itable->robot.robotHavePlante = true;
        //itable->planteStockFull[0].etat = false;
    });
    takeStock0->setCostAction(1,0,itable,0,0); //num action puis num zone de l'action
    listeAction.push_back(takeStock0);


     
// ACTION 2 PUT IN CONSTRUCTION ZONE 
    putInConstruction0->setStartPoint(0, 0, 90, Direction::FORWARD, Rotation::SHORTEST);
    putInConstruction0->setEndPoint(0, 0, 90, Direction::BACKWARD, Rotation::SHORTEST);
    putInConstruction0->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return construct(0, 0, 90);
    });
    putInConstruction0->goodEnd([](TableState*itable,CmdAsserv*irobot){
        //int16_t x,y,theta;
        //itable->robot.robotHavePlante = false;
        //itable->JardiniereFull[0].etat = true;
        //irobot->get_coordinates(x,y,theta);
        //irobot->set_coordinates(x,- 1500 + 130 ,90);
        //itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInConstruction0->setCostAction(2,0,itable,0,0);
    listeAction.push_back(putInConstruction0);

// ACTION 3
//    turnSolarPanelAction->setKeyMoment(65000);
//    turnSolarPanelAction->setStartPoint(810,(itable->robot.colorTeam == YELLOW ? 1100 : -1100),(itable->robot.colorTeam == YELLOW ? -90 : 90), Direction::FORWARD, Rotation::SHORTEST);
//    turnSolarPanelAction->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
//        return turnSolarPannel(itable, iAsser, iarduino);
//    });
//    turnSolarPanelAction->goodEnd([](TableState*itable,CmdAsserv*irobot){
//        itable->solarPanelTurn.etat = true;
//    });
//    turnSolarPanelAction->setCostAction(3,0,itable,0,0);
//    listeAction.push_back(turnSolarPanelAction);


        // ACTION 4
    returnToHomeAction->setStartPoint(700,(itable->robot.colorTeam == YELLOW ? 1200 : -1200), Direction::FORWARD, Rotation::SHORTEST);
    returnToHomeAction->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        int iret = 0;
        //if(FastReleasePlant(iarduino))
        //    iret = -100;
        return iret;
    });
    returnToHomeAction->setKeyMoment(85000);
    returnToHomeAction->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->incrementScore(RETURN_HOME);
    });
    returnToHomeAction->setCostAction(4,0,itable,0,0);
    listeAction.push_back(returnToHomeAction);

    //ACTION 5
    waitFin->setStartPoint(700,0,(itable->robot.colorTeam == YELLOW ? -90 : 90),Direction::FORWARD,Rotation::SHORTEST);
    waitFin->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable){
        while (itable->startTime+88000 > _millis()){
            sleep(0.25);
        }
        return 1;
    });
    waitFin->setCostAction(5,0,itable,0,0);
    listeAction.push_back(waitFin);    

    //Choose first action
    choosNextAction();
}

int actionContainer::actionContainerRun(CmdAsserv * robot,TableState* itable){
    int iActionReturn, iChoosNextReturn = 0, iRet = 0;
    iActionReturn = currentAction->runAction();
    if(iActionReturn == -100){
        iRet = -100;
    }
    else if(iActionReturn!=0){
        resetActionneur();
        initAction(robot, arduino, table);
    }
    // else if(forceNextAction()){
    //     resetActionneur();
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
        //LOG_INFO("elem = ", currentAction->getName(), "cost = ", cost);
        if(cost>bestCost){
            bestCost = cost;
            currentAction = elem;
        }
    }
    if(bestCost!=-1) {LOG_GREEN_INFO("CHOOSE : ",currentAction->getName(), " / cost : ",bestCost);}
    return bestCost;
}

void actionContainer::resetAllAction(void){
    for (action* elem : listeAction) {
        elem->resetActionEnable();
    }
}