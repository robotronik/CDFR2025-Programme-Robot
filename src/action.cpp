#include "action.hpp"


action::action(std::string name, robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
    mainRobot = imainRobot;
    robot = irobot;
    arduino = iarduino;
    table = itable;
    actionName = name;

    noEndPoint = true;
    currentState = FSMACTION_INIT;
    initStat = true;
    actionEnable = true;
    actionDone = false;

    actionPoint = 0;
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
        deplacementreturn = runActionPtr(this,mainRobot,robot,arduino,table);
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
    int cost = validActionPtr(table);
    if(actionEnable == false){
        cost = -1;
    }
    return cost;
}

void action::setCostAction(std::function<int(tableState*)> ptr){
    validActionPtr = ptr;
}

int action::setActionValue(){
    int value;
    return value;
}

void action::setRunAction(std::function<int(action*, robotCDFR*, Asser*, Arduino*, tableState*)> ptr){
    runActionPtr = ptr;
}

int action::goToStart(void){
    if(noTetaStart){
        return deplacementgoToPointNoTurn(*mainRobot, robot, startPostion.x,startPostion.y, startDirection,startRotation);
    }
    else{
        return deplacementgoToPoint(*mainRobot, robot, startPostion.x,startPostion.y, startPostion.teta, startDirection);
    }    
}


int action::goToEnd(void){
    return deplacementgoToPoint(*mainRobot, robot, endPostion.x, endPostion.y, endPostion.teta, endDirection);
}

void action::setStartPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation){
    startPostion.x = x;
    startPostion.y = y;
    startPostion.teta = teta;
    startDirection = Direction;
    startRotation = rotation;
}

void action::setStartPointNoTeta(int x, int y, asser_direction_side Direction, asser_rotation_side rotation){
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

bool action::isDone(void){
    return actionDone;
}

void action::setDone(bool done){
    actionDone = done;
}

void action::setObstacle(obstacle_t* obst, int id){
    obstacle = *obst;
    obstacle.centre_x = table->centre_action[id].x;
    obstacle.centre_y = table->centre_action[id].y;
}

void action::setZone(int dim, tableState *itable){
    cartesian_position_t z1,z2;
    z1.x = (int)((obstacle.centre_x+obstacle.demie_largeur)*2);
    z1.y = (int)((obstacle.centre_y+obstacle.demie_longueur)*2);
    z2.x = (int)((obstacle.centre_x-obstacle.demie_largeur)*2);
    z2.y = (int)((obstacle.centre_y-obstacle.demie_longueur)*2);

    zone = (int*)calloc((z1.x-z2.x+1)*(z1.y-z2.y+1),sizeof(int));
    int count = 0;
    for (int i = z2.x; i <= z1.x; i++)
    {
        for (int j = z2.y; i <= z1.y; j++)
        {
            zone[count] = i + dim*j;
            count++;
            itable-
        }
    }
    return;
}

obstacle_t* action::getObstacle(void){
    return &obstacle;
}

int action::getZone(void){
    return *zone;
}