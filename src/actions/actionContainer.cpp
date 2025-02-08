#include "actions/actionContainer.hpp"
#include "main.hpp"

ActionContainer::ActionContainer(){}
ActionContainer::~ActionContainer(){}

void ActionContainer::init(){
    takeStock0 = new Action("takeStock0");
    takeStock1 = new Action("takeStock1");
    takeStock2 = new Action("takeStock2");
    takeStock3 = new Action("takeStock3");
    takeStock4 = new Action("takeStock4");
    takeStock5 = new Action("takeStock5");
    takeStock6 = new Action("takeStock6");
    takeStock7 = new Action("takeStock7");
    takeStock8 = new Action("takeStock8");
    takeStock9 = new Action("takeStock9");

    putInConstruction0 = new Action("putInConstruction0");
    putInConstruction1 = new Action("putInConstruction1");
    putInConstruction2 = new Action("putInConstruction2");
    putInConstruction3 = new Action("putInConstruction3");
    putInConstruction4 = new Action("putInConstruction4");
    putInConstruction5 = new Action("putInConstruction5");

    waitFin = new Action("waitFin");
    returnToHomeAction = new Action("returnToHomeAction");

}

void ActionContainer::initAction(){
    
// ACTION 1 : CHERCHER DU STOCK
    takeStock0->setStartPoint(0,0,Direction::FORWARD,Rotation::SHORTEST); //replace 0,0 by coords of Stock[0]
    takeStock0->setRunAction([&](Action* iaction) {
        //return takePlant2(iAsser,iarduino,itable,plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY,plantPosition[0].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[0].y + MARGESTOCKPLANTY/DIVIDE,0);
        return takeStock(0,0,0,0,0);
    });
    takeStock0->goodEnd([](){
        //tableStatus.robot.robotHavePlante = true;
        //tableStatus.planteStockFull[0].etat = false;
    });
    takeStock0->setCostAction(1,0,0,0); //num action puis num zone de l'action
    listeAction.push_back(takeStock0);


     
// ACTION 2 PUT IN CONSTRUCTION ZONE 
    putInConstruction0->setStartPoint(0, 0, 90, Direction::FORWARD, Rotation::SHORTEST);
    putInConstruction0->setEndPoint(0, 0, 90, Direction::BACKWARD, Rotation::SHORTEST);
    putInConstruction0->setRunAction([](Action* iaction) {
        return construct(0, 0, 90);
    });
    putInConstruction0->goodEnd([](){
        //int16_t x,y,theta;
        //tableStatus.robot.robotHavePlante = false;
        //tableStatus.JardiniereFull[0].etat = true;
        //iasserv.get_coordinates(x,y,theta);
        //iasserv.set_coordinates(x,- 1500 + 130 ,90);
        //tableStatus.incrementScore(PLANTE_JARDINIERE);
    });
    putInConstruction0->setCostAction(2,0,0,0);
    listeAction.push_back(putInConstruction0);

// ACTION 3
//    turnSolarPanelAction->setKeyMoment(65000);
//    turnSolarPanelAction->setStartPoint(810,(tableStatus.robot.colorTeam == YELLOW ? 1100 : -1100),(tableStatus.robot.colorTeam == YELLOW ? -90 : 90), Direction::FORWARD, Rotation::SHORTEST);
//    turnSolarPanelAction->setRunAction([](action* iaction, Asserv* iAsser, Arduino* iarduino, TableState*itable) {
//        return turnSolarPannel(itable, iAsser, iarduino);
//    });
//    turnSolarPanelAction->goodEnd([](TableState*itable,Asserv*irobot){
//        tableStatus.solarPanelTurn.etat = true;
//    });
//    turnSolarPanelAction->setCostAction(3,0,itable,0,0);
//    listeAction.push_back(turnSolarPanelAction);


        // ACTION 4
    returnToHomeAction->setStartPoint(700,(tableStatus.robot.colorTeam == YELLOW ? 1200 : -1200), Direction::FORWARD, Rotation::SHORTEST);
    returnToHomeAction->setRunAction([](Action* iaction) {
        int iret = 0;
        //if(FastReleasePlant(iarduino))
        //    iret = -100;
        return iret;
    });
    returnToHomeAction->setKeyMoment(85000);
    returnToHomeAction->goodEnd([](){
        tableStatus.incrementScore(RETURN_HOME);
    });
    returnToHomeAction->setCostAction(4,0,0,0);
    listeAction.push_back(returnToHomeAction);

    //ACTION 5
    waitFin->setStartPoint(700,0,(tableStatus.robot.colorTeam == YELLOW ? -90 : 90),Direction::FORWARD,Rotation::SHORTEST);
    waitFin->setRunAction([](Action* iaction){
        while (tableStatus.startTime+88000 > _millis()){
            sleep(0.25);
        }
        return 1;
    });
    waitFin->setCostAction(5,0,0,0);
    listeAction.push_back(waitFin);    

    //Choose first action
    choosNextAction();
}

int ActionContainer::run(){
    int iActionReturn, iChoosNextReturn = 0, iRet = 0;
    iActionReturn = currentAction->runAction();
    if(iActionReturn == -100){
        iRet = -100;
    }
    else if(iActionReturn!=0){
        resetActionneur();
        initAction();
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


bool ActionContainer::forceNextAction(){
    bool bRet = false;
    for (Action* elem : listeAction) {
        if(elem->actionNeedForce()){
            bRet = true;
        }
    }
    return bRet;
}

int ActionContainer::choosNextAction(){
    LOG_GREEN_INFO("CHOOSE NEW ACTION: ");
    int bestCost = -1;
    for (Action* elem : listeAction) {
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

void ActionContainer::resetAllAction(){
    for (Action* elem : listeAction) {
        elem->resetActionEnable();
    }
}