#include "action.hpp"


action::action(std::string name, CmdAsserv* irobot, Arduino* iarduino, TableState* itable){
    robot = irobot;
    arduino = iarduino;
    table = itable;
    actionName = name;

    noEndPoint = true;
    currentState = FSM_ACTION_INIT;
    initStat = true;
    actionEnable = true;
}

action::~action(){
   
}

int action::runAction(void){
    LOG_SCOPE("Action");
    int ireturn = 0;
    fsmAction_t nextState = currentState;
    nav_return_t navRet;

    switch (currentState)
    {
    case FSM_ACTION_INIT :
        if(initStat) LOG_STATE("FSM_ACTION_INIT");
        nextState = FSM_ACTION_MOVESTART;
        break;

    case FSM_ACTION_MOVESTART :
        if(initStat) LOG_STATE("FSM_ACTION_MOVESTART");
        navRet = goToStart();
        if(navRet>0){
            nextState = FSM_ACTION_ACTION;
        }
        else if(navRet<0){
            nextState = FSM_ACTION_INIT;
            actionEnable = false;
            if(badEndPtr){
                badEndPtr(table);
            }
            ireturn = -1;
        }
        break;


    case FSM_ACTION_ACTION :
        if(initStat) LOG_STATE("FSM_ACTION_ACTION");
        navRet = runActionPtr(this,robot,arduino,table);
        if(navRet>0){
            if(noEndPoint){
                nextState = FSM_ACTION_INIT;
                ireturn = 1;
            }
            else{
                nextState = FSM_ACTION_MOVEEND;
            }
            if(goodEndPtr){
                goodEndPtr(table,robot);
            }
        }
        else if(navRet<0){
            nextState = FSM_ACTION_INIT;
            actionEnable = false;
            if(badEndPtr){
                badEndPtr(table);
            }
            ireturn = -1;
        }
        break;


    case FSM_ACTION_MOVEEND :
        if(initStat) LOG_STATE("FSM_ACTION_MOVEEND");
        navRet = goToEnd();
        if(navRet>0){
            nextState = FSM_ACTION_INIT;
            ireturn = 1;
        }
        else if(navRet<0){
            nextState = FSM_ACTION_INIT;
            actionEnable = false;
            if(badEndPtr){
                badEndPtr(table);
            }
            ireturn = -1;
        }
        break;
        
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = FSM_ACTION_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}

int action::costAction(void){
    int cost = validActionPtr;
    if(actionEnable == false){
        cost = -1;
    }
    return cost;
}

void action::setCostAction(int num_action, int num_i_action, TableState *itable, int x_start, int y_start){
    int distance_action;

    // TODO : Change this to use itable.robot.pos instead
    int16_t x_pos,y_pos,theta_pos;
    robot->get_coordinates(x_pos,y_pos,theta_pos);

    validActionPtr = -1;
    //ACTION 1 : TAKE STOCK
    //TODO
    //if (num_action == 1 && itable->planteStockFull[num_i_action].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) && itable->startTime+75000 > _millis()){
    //    distance_action = sqrt(pow(x_pos-x_start,2) + pow(y_pos-y_start,2));  //distance de l'action au robot
    //    validActionPtr = itable->planteStockFull[num_i_action].cout - distance_action/100; //distance : 10cm = -1 points
    //    LOG_GREEN_INFO("action 1 : ",validActionPtr," / ",num_i_action);
    //}

    //ACTION 2 : PUT IN CONSTRUCTION
    //TODO
    //else if (num_action == 2 && !itable->JardiniereFull[num_i_action].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == JardinierePosition[num_i_action].team && itable->jardiniereFree[num_i_action].etat){
    //    distance_action = sqrt(pow(x_pos-x_start,2) + pow(y_pos-y_start,2));
    //    validActionPtr = itable->JardiniereFull[num_i_action].cout - distance_action/100;
    //    LOG_GREEN_INFO("action 2 : ",validActionPtr," / ",num_i_action, " / ", distance_action);
    //}

    //ACTION 4 : return to Home
    if (num_action == 4 && itable->startTime+85000 < _millis()){
        validActionPtr = 200;
        LOG_GREEN_INFO("action 4 : ",validActionPtr," / ",num_i_action);
    }

    /*
    //ACTION 5 : wait until fin
    else if (num_action == 5 && itable->startTime+88000 > _millis() && !itable->FIN){
        validActionPtr = 4;
        LOG_GREEN_INFO("action 5 : ",validActionPtr);
    
    }
   */
}

void action::setRunAction(std::function<int(action*, CmdAsserv*, Arduino*, TableState*)> ptr){
    runActionPtr = ptr;
}

nav_return_t action::goToStart(void){
    if(nothetaStart){
        return navigationGoToNoTurn(startPostion.x,startPostion.y, startDirection,startRotation);
    }
    else{
        return navigationGoTo(startPostion.x,startPostion.y, startPostion.theta, startDirection);
    }    
}


nav_return_t action::goToEnd(void){
    return navigationGoTo(endPostion.x, endPostion.y, endPostion.theta, endDirection);
}

void action::setStartPoint(int x, int y, int theta, CmdAsserv::Direction Direction, CmdAsserv::Rotation rotation){
    startPostion.x = x;
    startPostion.y = y;
    startPostion.theta = theta;
    startDirection = Direction;
    startRotation = rotation;
}

void action::setStartPoint(int x, int y, CmdAsserv::Direction Direction, CmdAsserv::Rotation rotation){
    startPostion.x = x;
    startPostion.y = y;
    startDirection = Direction;
    startRotation = rotation;
    nothetaStart = true;
}

void action::setEndPoint(int x, int y, int theta, CmdAsserv::Direction Direction, CmdAsserv::Rotation rotation){
    endPostion.x = x;
    endPostion.y = y;
    endPostion.theta = theta;
    endDirection = Direction;
    endRotation = rotation;
    noEndPoint = false;
}

std::string action::getName(void){
    return actionName;
}

void action::goodEnd(std::function<void(TableState*,CmdAsserv*)> ptr){
    goodEndPtr = ptr;
}
void action::badEnd(std::function<void(TableState*)> ptr){
    badEndPtr = ptr;
}

void action::resetActionEnable(void){
    actionEnable = true;
}

void action::setKeyMoment(unsigned long keyMom){
    keyMomentSet = true;
    keyMoment = keyMom;
}

bool action::actionNeedForce(void){
    return table->startTime+keyMoment < _millis() && keyMomentSet;
}