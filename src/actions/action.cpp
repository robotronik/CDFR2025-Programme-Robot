#include "actions/action.hpp"
#include "main.hpp"

Action::Action(std::string name)
{
    actionName = name;

    noEndPoint = true;
    currentState = FSM_ACTION_INIT;
    initStat = true;
    actionEnable = true;
}

Action::~Action()
{
}

int Action::runAction(void)
{
    int ireturn = 0;
    fsmAction_t nextState = currentState;
    nav_return_t navRet;

    switch (currentState)
    {
    case FSM_ACTION_INIT:
        nextState = FSM_ACTION_MOVESTART;
        break;

    case FSM_ACTION_MOVESTART:
        navRet = goToStart();
        if (navRet > 0)
        {
            nextState = FSM_ACTION_ACTION;
        }
        else if (navRet < 0)
        {
            nextState = FSM_ACTION_INIT;
            actionEnable = false;
            if (badEndPtr)
            {
                badEndPtr();
            }
            ireturn = -1;
        }
        break;

    case FSM_ACTION_ACTION:
        navRet = (nav_return_t)runActionPtr(this);
        if (navRet > 0)
        {
            if (noEndPoint)
            {
                nextState = FSM_ACTION_INIT;
                ireturn = 1;
            }
            else
            {
                nextState = FSM_ACTION_MOVEEND;
            }
            if (goodEndPtr)
            {
                goodEndPtr();
            }
        }
        else if (navRet < 0)
        {
            nextState = FSM_ACTION_INIT;
            actionEnable = false;
            if (badEndPtr)
            {
                badEndPtr();
            }
            ireturn = -1;
        }
        break;

    case FSM_ACTION_MOVEEND:
        navRet = goToEnd();
        if (navRet > 0)
        {
            nextState = FSM_ACTION_INIT;
            ireturn = 1;
        }
        else if (navRet < 0)
        {
            nextState = FSM_ACTION_INIT;
            actionEnable = false;
            if (badEndPtr)
            {
                badEndPtr();
            }
            ireturn = -1;
        }
        break;

    default:
        nextState = FSM_ACTION_INIT;
        break;
    }

    initStat = false;
    if (nextState != currentState)
    {
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}

int Action::costAction(void)
{
    int cost = validActionPtr;
    if (actionEnable == false)
    {
        cost = -1;
    }
    return cost;
}

void Action::setCostAction(int num_action, int num_i_action, int x_start, int y_start)
{
    int distance_action;

    // TODO : Change this to use itable.robot.pos instead
    int16_t x_pos, y_pos, theta_pos;
    asserv.get_coordinates(x_pos, y_pos, theta_pos);

    validActionPtr = -1;
    // ACTION 1 : TAKE STOCK
    // TODO
    // if (num_action == 1 && tableStatus.planteStockFull[num_i_action].etat && !tableStatus.robot.robotHavePlante && !allJardiniereFull(itable) && tableStatus.startTime+75000 > _millis()){
    //     distance_action = sqrt(pow(x_pos-x_start,2) + pow(y_pos-y_start,2));  //distance de l'action au robot
    //     validActionPtr = tableStatus.planteStockFull[num_i_action].cout - distance_action/100; //distance : 10cm = -1 points
    //     LOG_GREEN_INFO("Action 1 : ",validActionPtr," / ",num_i_action);
    // }

    // ACTION 2 : PUT IN CONSTRUCTION
    // TODO
    // else if (num_action == 2 && !tableStatus.JardiniereFull[num_i_action].etat && tableStatus.robot.robotHavePlante && tableStatus.robot.colorTeam == JardinierePosition[num_i_action].team && tableStatus.jardiniereFree[num_i_action].etat){
    //     distance_action = sqrt(pow(x_pos-x_start,2) + pow(y_pos-y_start,2));
    //     validActionPtr = tableStatus.JardiniereFull[num_i_action].cout - distance_action/100;
    //     LOG_GREEN_INFO("Action 2 : ",validActionPtr," / ",num_i_action, " / ", distance_action);
    // }

    // ACTION 4 : return to Home
    if (num_action == 4 && tableStatus.startTime + 85000 < _millis())
    {
        validActionPtr = 200;
        LOG_GREEN_INFO("Action 4 : ", validActionPtr, " / ", num_i_action);
    }

    /*
    //ACTION 5 : wait until fin
    else if (num_action == 5 && tableStatus.startTime+88000 > _millis() && !tableStatus.FIN){
        validActionPtr = 4;
        LOG_GREEN_INFO("Action 5 : ",validActionPtr);

    }
   */
}

void Action::setRunAction(std::function<int(Action *)> ptr)
{
    runActionPtr = ptr;
}

nav_return_t Action::goToStart(void)
{
    if (nothetaStart)
    {
        return navigationGoToNoTurn(startPostion.x, startPostion.y, startDirection, startRotation);
    }
    else
    {
        return navigationGoTo(startPostion.x, startPostion.y, startPostion.theta, startDirection);
    }
}

nav_return_t Action::goToEnd(void)
{
    return navigationGoTo(endPostion.x, endPostion.y, endPostion.theta, endDirection);
}

void Action::setStartPoint(int x, int y, int theta, Direction Direction, Rotation rotation)
{
    startPostion.x = x;
    startPostion.y = y;
    startPostion.theta = theta;
    startDirection = Direction;
    startRotation = rotation;
}

void Action::setStartPoint(int x, int y, Direction Direction, Rotation rotation)
{
    startPostion.x = x;
    startPostion.y = y;
    startDirection = Direction;
    startRotation = rotation;
    nothetaStart = true;
}

void Action::setEndPoint(int x, int y, int theta, Direction Direction, Rotation rotation)
{
    endPostion.x = x;
    endPostion.y = y;
    endPostion.theta = theta;
    endDirection = Direction;
    endRotation = rotation;
    noEndPoint = false;
}

std::string Action::getName(void)
{
    return actionName;
}

void Action::goodEnd(std::function<void()> ptr)
{
    goodEndPtr = ptr;
}
void Action::badEnd(std::function<void()> ptr)
{
    badEndPtr = ptr;
}

void Action::resetActionEnable(void)
{
    actionEnable = true;
}

// idk what this does
void Action::setKeyMoment(unsigned long keyMom)
{
    keyMomentSet = true;
    keyMoment = keyMom;
}

bool Action::actionNeedForce(void)
{
    return tableStatus.startTime + keyMoment < _millis() && keyMomentSet;
}