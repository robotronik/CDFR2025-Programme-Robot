#include "action.hpp"


action::action(std::string name, Asser* irobot, Arduino* iarduino, tableState* itable){
    robot = irobot;
    arduino = iarduino;
    table = itable;
    actionName = name;

    noEndPoint = true;
    currentState = FSMACTION_INIT;
    initStat = true;
    actionEnable = true;
}

action::~action(){
   
}

int action::runAction(void){
    LOG_SCOPE("Action");
    int ireturn = 0;
    fsmAction_t nextState = currentState;
    int deplacementreturn;

    switch (currentState)
    {
    case FSMACTION_INIT :
        if(initStat) LOG_STATE("FSMACTION_INIT");
        nextState = FSMACTION_MOVESTART;
        break;

    case FSMACTION_MOVESTART :
        if(initStat) LOG_STATE("FSMACTION_MOVESTART");
        deplacementreturn = goToStart();
        if(deplacementreturn>0){
            nextState = FSMACTION_ACTION;
        }
        else if(deplacementreturn<0){
            nextState = FSMACTION_INIT;
            actionEnable = false;
            if(badEndPtr){
                badEndPtr(table);
            }
            ireturn = -1;
        }
        break;


    case FSMACTION_ACTION :
        if(initStat) LOG_STATE("FSMACTION_ACTION");
        deplacementreturn = runActionPtr(this,robot,arduino,table);
        if(deplacementreturn>0){
            if(noEndPoint){
                nextState = FSMACTION_INIT;
                ireturn = 1;
            }
            else{
                nextState = FSMACTION_MOVEEND;
            }
            if(goodEndPtr){
                goodEndPtr(table);
            }
        }
        else if(deplacementreturn<0){
            nextState = FSMACTION_INIT;
            actionEnable = false;
            if(badEndPtr){
                badEndPtr(table);
            }
            ireturn = -1;
        }
        break;


    case FSMACTION_MOVEEND :
        if(initStat) LOG_STATE("FSMACTION_MOVEEND");
        deplacementreturn = goToEnd();
        if(deplacementreturn>0){
            nextState = FSMACTION_INIT;
            ireturn = 1;
        }
        else if(deplacementreturn<0){
            nextState = FSMACTION_INIT;
            actionEnable = false;
            if(badEndPtr){
                badEndPtr(table);
            }
            ireturn = -1;
        }
        break;
        
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = FSMACTION_INIT;
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

void action::setCostAction(int num_action, int num_i_action, tableState *itable){
    int x,y,theta,distance_action;
    robot->getCoords(x,y,theta);
    validActionPtr = -1;
    //ACTION 1 : TAKE PLANT
    if (num_action == 1 && itable->planteStockFull[num_i_action].etat && !itable->robot.robotHavePlante && !allJardiniereFull(itable) ){
        distance_action = sqrt(pow(x-plantPosition[num_i_action].x,2) + pow(y-plantPosition[num_i_action].y,2));  //distance de l'action au robot
        validActionPtr = itable->planteStockFull[num_i_action].cout - distance_action/100; //distance : 10cm = -1 points
        LOG_GREEN_INFO("action 1 : ",validActionPtr," / ",num_i_action);
    }
    //ACTION 2 : PutInJardinière
    else if (num_action == 2 && !itable->JardiniereFull[num_i_action].etat && itable->robot.robotHavePlante && itable->robot.colorTeam == JardinierePosition[num_i_action].team && itable->jardiniereFree[num_i_action].etat){
        distance_action = sqrt(pow(x-JardinierePosition[num_i_action].x,2) + pow(y-JardinierePosition[num_i_action].y,2));
        validActionPtr = itable->JardiniereFull[num_i_action].cout - distance_action/100;
        LOG_GREEN_INFO("action 2 : ",validActionPtr," / ",num_i_action, " / ", distance_action);
    }
    //ACTION 3 : turn SolarPanel
    else if (num_action == 3 && !itable->solarPanelTurn.etat){
        if (itable->startTime+65000 < millis() || allJardiniereFull(itable)){
        validActionPtr = itable->solarPanelTurn.cout;}
        else { validActionPtr = itable->solarPanelTurn.cout/10;}
        LOG_GREEN_INFO("action 3 : ",validActionPtr," / ",num_i_action);
    }

    //ACTION 6 : PushPot
    else if (num_action == 6 && itable->robot.colorTeam == JardinierePosition[num_i_action].team && !itable->jardiniereFree[num_i_action].etat){
        distance_action = sqrt(pow(x-JardinierePosition[num_i_action].x,2) + pow(y-JardinierePosition[num_i_action].y,2));
        validActionPtr = itable->jardiniereFree[num_i_action].cout - distance_action/100; 
        LOG_GREEN_INFO("action 6 : ",validActionPtr," / ",num_i_action);
    }
   
}

void action::setRunAction(std::function<int(action*, Asser*, Arduino*, tableState*)> ptr){
    runActionPtr = ptr;
}

int action::goToStart(void){
    if(noTetaStart){
        return deplacementgoToPointNoTurn(table->robot.collide, robot, startPostion.x,startPostion.y, startDirection,startRotation);
    }
    else{
        return deplacementgoToPoint(table->robot.collide, robot, startPostion.x,startPostion.y, startPostion.teta, startDirection);
    }    
}


int action::goToEnd(void){
    return deplacementgoToPoint(table->robot.collide, robot, endPostion.x, endPostion.y, endPostion.teta, endDirection);
}

void action::setStartPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation){
    startPostion.x = x;
    startPostion.y = y;
    startPostion.teta = teta;
    startDirection = Direction;
    startRotation = rotation;
}

void action::setStartPoint(int x, int y, asser_direction_side Direction, asser_rotation_side rotation){
    startPostion.x = x;
    startPostion.y = y;
    startDirection = Direction;
    startRotation = rotation;
    noTetaStart = true;
}

void action::setEndPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation){
    endPostion.x = x;
    endPostion.y = y;
    endPostion.teta = teta;
    endDirection = Direction;
    endRotation = rotation;
    noEndPoint = false;
}

std::string action::getName(void){
    return actionName;
}

void action::goodEnd(std::function<void(tableState*)> ptr){
    goodEndPtr = ptr;
}
void action::badEnd(std::function<void(tableState*)> ptr){
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
    return table->startTime+keyMoment < millis() && keyMomentSet;
}