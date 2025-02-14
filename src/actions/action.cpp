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
    ReturnFSM_t ret;
    switch (runState)
    {
    //****************************************************************
    case FSM_ACTION_GATHER:
        ret = TakeSingleStockFSM(0); //TODO choose the stock
        if (ret == FSM_RETURN_DONE){
            if (true) // TODO Done collecting
                runState = FSM_ACTION_BUILD;
            // else choose another stock
        }
        else if (ret == FSM_RETURN_ERROR){
            LOG_ERROR("Couldn't take stock");
            // TODO Handle error
        }
        break;
    //****************************************************************
    case FSM_ACTION_BUILD:
        ret = ConstructAllTribunesFSM(0); //TODO choose the zone
        if (ret == FSM_RETURN_DONE){
            runState = FSM_ACTION_NAV_HOME;
        }
        else if (ret == FSM_RETURN_ERROR){
            LOG_ERROR("Couldn't build tribune");
            // TODO Handle error
        }
        break;
    //****************************************************************
    case FSM_ACTION_DEPLOY_BANNER:
        runState = FSM_ACTION_NAV_HOME;
        break;
    //****************************************************************
    case FSM_ACTION_NAV_HOME:
        // Maybe do something like
        // get close to home then move in if theres less than 3s left to the clock
        if (returnToHome()){
            runState = FSM_ACTION_GATHER;
            return true; //Robot is done
        }
        break;
    }
    return false;
}

ReturnFSM_t ActionFSM::TakeSingleStockFSM(int num){
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
ReturnFSM_t ActionFSM::ConstructAllTribunesFSM(int zone){
    static int num = 0; // Keep track of the tribune were building
    switch (constructAllTribunesState){
    case FSM_CONSTRUCT_NAV:
        break;
    case FSM_CONSTRUCT_MOVE:
        // Place the robot to a tribune building location
        break;
    case FSM_CONSTRUCT_BUILD:
        if (constructSingleTribune()){
            builtTribuneHeights[num]++;
            if (false) { // TODO revolver is empty or plank_count == 0
                constructAllTribunesState = FSM_CONSTRUCT_EXIT;
            }
            else if (builtTribuneHeights[num] == 3){
                constructAllTribunesState = FSM_CONSTRUCT_MOVE;
                num++;
            }
        }
        break;
    case FSM_CONSTRUCT_EXIT:
        // Get out the way to make sure we dont destroy shit
        // if nav_done
        //   constructAllTribunesState = FSM_CONSTRUCT_NAV;
        //   return true;
        break;
    }
    return FSM_RETURN_WORKING;
}