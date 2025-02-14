#include <string>
#include "actions/action.hpp"
#include "utils/logger.hpp"
#include "main.hpp"
#include "navigation/navigation.h"
#include "actions/functions.h"
#include "defs/tableState.hpp"
#include "defs/constante.h"

ActionFSM::ActionFSM(){
    Reset();
}

ActionFSM::~ActionFSM(){}

void ActionFSM::Reset(){
    runState = FSM_ACTION_GATHER;
}

bool ActionFSM::RunFSM(){
    switch (runState)
    {
    //****************************************************************
    case FSM_ACTION_GATHER:
        runState = FSM_ACTION_BUILD;
        break;
    //****************************************************************
    case FSM_ACTION_BUILD:
        runState = FSM_ACTION_DEPLOY_BANNER;
        break;
    //****************************************************************
    case FSM_ACTION_DEPLOY_BANNER:
        runState = FSM_ACTION_NAV_HOME;
        break;
    //****************************************************************
    case FSM_ACTION_NAV_HOME:
        if (returnToHome()){
            runState = FSM_ACTION_GATHER;
            return true; //Robot is done
        }
        break;
    }
    return false;
}

ReturnFSM_t TakeSingleStockFSM(int num){
    switch (takeSingleStockState){
    case FSM_SINGLESTOCK_NAV:
        // navigationGoTo(startPostion.x, startPostion.y, startPostion.theta, startDirection);
        break;
    case FSM_SINGLESTOCK_PREPARE:
        // Finish prep of the revolver in case its not done yet
        break;
    case FSM_SINGLESTOCK_MOVE:
        // Move while spinning the revolver (shit is gonna get real)
        // navigationGoToNoTurn(startPostion.x, startPostion.y, startDirection, startRotation);
        break;
    case FSM_SINGLESTOCK_COLLECT:
        // Collect the stock
        if (takeStockPlatforms()){
            takeSingleStockState = FSM_SINGLESTOCK_NAV;
            setStockAsRemoved(num);
            return FSM_RETURN_DONE;
        }
        break;
    }
    return FSM_RETURN_WORKING;
}
ReturnFSM_t ConstructAllTribunesFSM(int zone){
    switch (constructAllTribunesState){
    case FSM_CONSTRUCT_NAV:
        break;
    case FSM_CONSTRUCT_BUILD:
        if (constructSingleTribune()){
            constructAllTribunesState = FSM_CONSTRUCT_MOVE;
        }
        break;
    case FSM_CONSTRUCT_MOVE:
        // Replace the robot to build another tribune
        break;
    }
    return FSM_RETURN_WORKING;
}