#include "deplacement.h"



int deplacementLinearPoint(robotCDFR mainRobot, Asser* robot, int x, int y){
    LOG_SCOPE("MOVE");
    static unsigned long startTime;
    static int memx;
    static int memy;
    static bool initStat = true; 
    static deplcement_State_t step = DEPLACEMENT_INIT;
    deplcement_State_t nextstep = step;
    int iret = 0;
    int distance;


    switch (step)
    {
    case DEPLACEMENT_INIT:
        if(initStat) LOG_STATE("DEPLACEMENT_INIT");
        memx = x;
        memy = y;

        // TOFIX in future
        // Go directly in wait if collide
        // Actual problem : at the begengin, the collide system not watch the good side

        nextstep = DEPLACEMENT_WAITFIRSTMOVE;
        robot->linearSetpoint(memx,memy);

        // if(mainRobot.tableStatus.collide < DISTANCESTOP){
        //     printf("distance colide : %d\n",mainRobot.tableStatus.collide);
        //     nextstep = DEPLACEMENT_WAIT;
        //     startTime = millis() + 5000; //TIME waiting
        // }
        // else{
        //     nextstep = DEPLACEMENT_MOVE;
        //     robot->linearSetpoint(memx,memy);
        // }
        break;
    case DEPLACEMENT_WAITFIRSTMOVE:
        if(initStat) LOG_STATE("DEPLACEMENT_WAITFIRSTMOVE");
        robot->getBrakingDistance(distance);
        if(distance != 0){
            if(mainRobot.collide < DISTANCESTOP){
                LOG_INFO("distance colide : ",mainRobot.collide);
                nextstep = DEPLACEMENT_WAIT;
                startTime = millis() + 5000; //TIME waiting
            }
            else{
                nextstep = DEPLACEMENT_MOVE;
                robot->linearSetpoint(memx,memy);
            }
        }
        break;

    case DEPLACEMENT_MOVE:
        if(initStat) LOG_STATE("DEPLACEMENT_MOVE");
        if(!robot->getError(LINEAR_ERROR)){
            nextstep = DEPLACEMENT_INIT;
            iret = 1; //GOOD END
        }
        if(mainRobot.collide < DISTANCESTOP){
            LOG_INFO("distance colide : ",mainRobot.collide);
            nextstep = DEPLACEMENT_STOP;
            robot->stop();
            robot->brakeMotor(true);
        }
        break;

    case DEPLACEMENT_STOP:
        if(initStat) LOG_STATE("DEPLACEMENT_STOP");
        robot->getBrakingDistance(distance);
        if(distance==0){
            startTime = millis() + 5000000;
            nextstep = DEPLACEMENT_WAIT;
        }
        break;

    case DEPLACEMENT_WAIT:
        if(initStat) LOG_STATE("DEPLACEMENT_WAIT");
        if(startTime < millis()){
            nextstep = DEPLACEMENT_INIT;
            iret = -1; //BAD END
        }
        if(mainRobot.collide > DISTANCERESTART){
            LOG_INFO("distance colide : ",mainRobot.collide);
            nextstep = DEPLACEMENT_MOVE;
            robot->brakeMotor(false);
            robot->enableMotor(true);
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    default:
        LOG_ERROR("DEFAULT");
        nextstep = DEPLACEMENT_INIT;
        break;
    }

    initStat = false;
    if(nextstep != step){
        initStat = true;
    }
    step = nextstep;
    return iret;
}



int deplacementgoToPoint(robotCDFR mainRobot, Asser* robot, int x, int y, int teta, asser_direction_side direction,asser_rotation_side rotationLookAt,asser_rotation_side rotation){
    LOG_SCOPE("go to");
    int ireturn = 0;
    static bool initStat = true;
    static go_to_State_t currentState = GOTO_INIT;
    go_to_State_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    case GOTO_INIT :
        if(initStat) LOG_STATE("GOTO_INIT");
        nextState = GOTO_LOOKAT;
        break;
    case GOTO_LOOKAT :
        if(initStat){ 
            LOG_STATE("GOTO_LOOKAT");
            if(direction == MOVE_FORWARD){
                robot->setLookForward(x,y,ROTATION_DIRECT);
            } 
            else{
                robot->setLookBackward(x,y,ROTATION_DIRECT);
            }
        }
        if(robot->getError(ANGULAR_ERROR)==0){
            nextState = GOTO_MOVE;
        }
        break;
    case GOTO_MOVE :
        if(initStat) LOG_STATE("GOTO_MOVE");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,x,y);
        if(deplacementreturn>0){
            nextState = GOTO_TURN;
        }
        else if(deplacementreturn<0){
            nextState = GOTO_INIT;
            ireturn = deplacementreturn;
        }
        break;
    case GOTO_TURN :
        if(initStat){ LOG_STATE("GOTO_TURN");
            robot->angularSetpoint(teta,rotation);
        }
        if(!robot->getError(ANGULAR_ERROR)){
            nextState = GOTO_INIT;
            ireturn = 1;
        }
        break;
    
    default:
        if(initStat) LOG_ERROR("default");
        nextState = GOTO_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int deplacementgoToPointNoTurn(robotCDFR mainRobot, Asser* robot, int x, int y, asser_direction_side direction,asser_rotation_side rotationLookAt){
    LOG_SCOPE("go to");
    int ireturn = 0;
    static bool initStat = true;
    static go_to_State_t currentState = GOTO_INIT;
    go_to_State_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    case GOTO_INIT :
        if(initStat) LOG_STATE("GOTO_INIT");
        nextState = GOTO_LOOKAT;
        break;
    case GOTO_LOOKAT :
        if(initStat){ 
            LOG_STATE("GOTO_LOOKAT");
            if(direction == MOVE_FORWARD){
                robot->setLookForward(x,y,ROTATION_DIRECT);
            } 
            else{
                robot->setLookBackward(x,y,ROTATION_DIRECT);
            }
        }
        if(robot->getError(ANGULAR_ERROR)==0){
            nextState = GOTO_MOVE;
        }
        break;
    case GOTO_MOVE :
        if(initStat) LOG_STATE("GOTO_MOVE");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,x,y);
        if(deplacementreturn>0){
            nextState = GOTO_INIT;
            ireturn = 1;
        }
        else if(deplacementreturn<0){
            nextState = GOTO_INIT;
            ireturn = -1;
        }
        break;
    default:
        if(initStat) LOG_ERROR("default");
        nextState = GOTO_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}