#include <string>
#include <exception>
#include "actions/action.hpp"
#include "utils/logger.hpp"
#include "main.hpp"
#include "navigation/navigation.h"
#include "actions/functions.h"
#include "actions/strats.hpp"
#include "defs/tableState.hpp"
#include "defs/constante.h"
#include "actions/revolver.hpp"

ActionFSM::ActionFSM(){
    Reset();
}

ActionFSM::~ActionFSM(){}

void ActionFSM::Reset(){
    runState = FSM_ACTION_GATHER;
    gatherStockState = FSM_GATHER_NAV;
    constructAllTribunesState = FSM_CONSTRUCT_NAV;
    initRevolver();
}

bool ActionFSM::RunFSM(){
    ReturnFSM_t ret;
    switch (runState)
    {
    //****************************************************************
    case FSM_ACTION_GATHER:
        ret = GatherStock();
        if (ret == FSM_RETURN_DONE)
            runState = FSM_ACTION_BUILD;
        else if (ret == FSM_RETURN_ERROR){
            LOG_ERROR("Couldn't take stock");
            // TODO Handle error
        }
        break;
    //****************************************************************
    case FSM_ACTION_BUILD:
        ret = ConstructAllTribunesFSM(); //TODO choose the zone
        if (ret == FSM_RETURN_DONE)
            runState = FSM_ACTION_NAV_HOME;
        else if (ret == FSM_RETURN_ERROR){
            LOG_ERROR("Couldn't build tribune");
            // TODO Handle error
        }
        break;
    //****************************************************************
    case FSM_ACTION_DEPLOY_BANNER:
        runState = FSM_ACTION_NAV_HOME;
        tableStatus.done_banderole = true;
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


ReturnFSM_t ActionFSM::GatherStock(){

    static int num = -1;
    static int offset;
    if (num == -1){
        if (!StratGather(num, offset)){
            return FSM_RETURN_DONE;
        }
    }

    position_t stockPos = STOCK_POSITION_ARRAY[num];
    int off = STOCK_OFFSET_MAPPING[num][offset];
    if (off < 0) return FSM_RETURN_ERROR;
    position_t stockOff = STOCK_OFFSETS[off];
    stock_direction_t stock_dir = STOCK_DIRECTION[num][offset]; // FORWARDS OR BACKWARDS
    Direction stock_nav_dir      = (stock_dir == FORWARDS) ? Direction::FORWARD : Direction::BACKWARD;
    direction_t stock_intake_dir = (stock_dir == FORWARDS) ? FROM_LEFT : FROM_RIGHT;
    nav_return_t nav_ret;
    switch (gatherStockState){
    case FSM_GATHER_NAV:
        // TODO Highways should be enabled
        nav_ret = navigationGoTo(stockPos.x + stockOff.x, stockPos.y + stockOff.y, stockOff.theta, Direction::SHORTEST, Rotation::SHORTEST, Rotation::SHORTEST, false);
        if (RevolverPrepareLowBarrel(stock_intake_dir) && (nav_ret == NAV_DONE)){
            gatherStockState = FSM_GATHER_MOVE;
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_GATHER_MOVE:
        if (stockPos.theta == 0) // Horizontal stock
            nav_ret = navigationGoToNoTurn(stockPos.x + stockOff.x, stockPos.y - stockOff.y/6, stock_nav_dir, Rotation::SHORTEST, false);
        else // Vertical stock
            nav_ret = navigationGoToNoTurn(stockPos.x - stockOff.x/6, stockPos.y + stockOff.y, stock_nav_dir, Rotation::SHORTEST, false);
        if ((nav_ret == NAV_DONE) & RevolverLoadStock(stock_intake_dir, num)){
            gatherStockState = FSM_GATHER_COLLECT;
        }
        else if (nav_ret == NAV_ERROR){
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_GATHER_COLLECT:
        // Collect the stock
        if (takeStockPlatforms()){
            gatherStockState = FSM_GATHER_NAV;
            setStockAsRemoved(num);
            num = -1;
            return FSM_RETURN_WORKING;
        }
        break;
    }
    return FSM_RETURN_WORKING;
}
ReturnFSM_t ActionFSM::ConstructAllTribunesFSM(){
    static int num = 0; // Keep track of the tribune were building
    static int zoneNum = -1; // Keep track of the zone were building in

    if (zoneNum == -1){
        StratConstruct(zoneNum);
    }

    /*
    position_t baseBuildPos = TRIBUNE_CONSTRUCT_POSITION[zoneNum];
    // Offset the build pos by 120mm * num
    int offset = 120 * num;
    baseBuildPos.x += 0;
    baseBuildPos.y += (tableStatus.robot.colorTeam == BLUE) ? -offset : +offset;
    */
   
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
        constructAllTribunesState = FSM_CONSTRUCT_NAV;
        return FSM_RETURN_DONE;
        break;
    }
    return FSM_RETURN_WORKING;
}