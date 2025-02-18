#include <string>
#include <exception>
#include "actions/action.hpp"
#include "utils/logger.hpp"
#include "main.hpp"
#include "navigation/navigation.h"
#include "actions/functions.h"
#include "defs/tableState.hpp"
#include "defs/constante.h"
#include "actions/revolver.hpp"

ActionFSM::ActionFSM(){
    Reset();
}

ActionFSM::~ActionFSM(){}

void ActionFSM::Reset(){
    runState = FSM_ACTION_GATHER;
    takeSingleStockState = FSM_SINGLESTOCK_NAV;
    constructAllTribunesState = FSM_CONSTRUCT_NAV;
    initRevolver();
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

ReturnFSM_t ActionFSM::TakeSingleStockFSM(int num, int offset){
    position_t stockPos = STOCK_POSITION_ARRAY[num];
    position_t stockOff = STOCK_OFFSETS[num][offset];
    stock_direction_t stock_dir = STOCK_DIRECTION[num][offset]; // FORWARDS OR BACKWARDS
    Direction stock_nav_dir      = stock_dir == FORWARDS ? Direction::FORWARD : Direction::BACKWARD;
    direction_t stock_intake_dir = stock_dir == FORWARDS ? FROM_LEFT : FROM_RIGHT;
    nav_return_t nav_ret;
    switch (takeSingleStockState){
    case FSM_SINGLESTOCK_NAV:
        nav_ret = navigationGoTo(stockPos.x + stockOff.x, stockPos.y + stockOff.y, stockOff.theta, Direction::FORWARD, Rotation::SHORTEST, Rotation::SHORTEST, true);
        if (nav_ret == NAV_DONE & RevolverPrepareLowBarrel(stock_intake_dir)){
            takeSingleStockState = FSM_SINGLESTOCK_MOVE;
        }
        else if (nav_ret == NAV_ERROR){
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_SINGLESTOCK_MOVE:
        if (stockPos.theta == 0) // Horizontal stock
            nav_ret = navigationGoToNoTurn(stockPos.x + stockOff.x, stockPos.y, stock_nav_dir, Rotation::SHORTEST, false);
        else // Vertical stock
            nav_ret = navigationGoToNoTurn(stockPos.x, stockPos.y + stockOff.y, stock_nav_dir, Rotation::SHORTEST, false);
        if (nav_ret == NAV_DONE & RevolverLoadStock(stock_intake_dir)){
            takeSingleStockState = FSM_SINGLESTOCK_COLLECT;
        }
        else if (nav_ret == NAV_ERROR){
            return FSM_RETURN_ERROR;
        }
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
        // Nav to the tribune building location (zone)
        break;
    case FSM_CONSTRUCT_MOVE:
        // Place the robot to a tribune building location
        break;
    case FSM_CONSTRUCT_PREPREVOLVER:
        if (RevolverRelease()){
            constructAllTribunesState = FSM_CONSTRUCT_BUILD;
        }
        break;
    case FSM_CONSTRUCT_BUILD:
        if (constructSingleTribune()){
            tableStatus.builtTribuneHeights[num]++;
            if (isRevolverEmpty() || tableStatus.robot.plank_count == 0) {
                constructAllTribunesState = FSM_CONSTRUCT_EXIT;
            }
            else if (tableStatus.builtTribuneHeights[num] == 3){
                constructAllTribunesState = FSM_CONSTRUCT_MOVE;
                num++;
            }
            else {
                constructAllTribunesState = FSM_CONSTRUCT_PREPREVOLVER;
            }
        }
        break;
    case FSM_CONSTRUCT_EXIT:
        // Get out the way to make sure we dont destroy shit
        // if nav_done
        //   constructAllTribunesState = FSM_CONSTRUCT_NAV;
        //   return true;
        constructAllTribunesState = FSM_CONSTRUCT_NAV
        return true;
        break;
    }
    return FSM_RETURN_WORKING;
}