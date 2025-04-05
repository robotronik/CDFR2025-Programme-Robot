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
    unsigned long ms = _millis();
    unsigned long maxGather, maxBuild;
    StratTimes(maxGather, maxBuild);
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
        ret = ConstructAllTribunesFSM();
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
        if (returnToHome()){
            runState = FSM_ACTION_GATHER;
            return true; // Robot is done
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
    static unsigned long startTime; // Start time of revolverLoading
    switch (gatherStockState){
    case FSM_GATHER_NAV:
        // TODO Highways should be enabled
        nav_ret = navigationGoTo(stockPos.x + stockOff.x, stockPos.y + stockOff.y, stockOff.theta, Direction::SHORTEST, Rotation::SHORTEST, Rotation::SHORTEST, false);
        if (RevolverPrepareLowBarrel(stock_intake_dir) && (nav_ret == NAV_DONE)){
            gatherStockState = FSM_GATHER_MOVE;
            asserv.set_linear_max_speed(160, 500, 500);
            startTime = _millis();
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_GATHER_MOVE:
    {
        if (stockPos.theta == 0) // Horizontal stock
            nav_ret = navigationGoToNoTurn(stockPos.x + stockOff.x, stockPos.y - stockOff.y/6, stock_nav_dir, Rotation::SHORTEST, false);
        else // Vertical stock
            nav_ret = navigationGoToNoTurn(stockPos.x - stockOff.x/6, stockPos.y + stockOff.y, stock_nav_dir, Rotation::SHORTEST, false);

        bool revolverDone = false;
        if (_millis() > startTime + 500)
            revolverDone = RevolverLoadStock(stock_intake_dir, num);
        if ((nav_ret == NAV_DONE) & revolverDone){
            gatherStockState = FSM_GATHER_COLLECT;
            asserv.set_linear_max_speed(10000, 500, 500);
        }
        else if (nav_ret == NAV_ERROR){
            return FSM_RETURN_ERROR;
        }
        break;
    }
    case FSM_GATHER_COLLECT:
        // Collect the stock
        if (takeStockPlatforms()){
            gatherStockState = FSM_GATHER_NAV;
            setStockAsRemoved(num);
            tableStatus.robot.plank_count += 2;
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

    static bool revolverReady = false;
    static bool liftReady = false;

    if (zoneNum == -1){
        StratConstruct(zoneNum);
    }

    // Offset the build pos by 120mm * num
    const int tribunesOffset = 120;
    position_t buildPos = TRIBUNE_CONSTRUCT_POSITION[zoneNum];
    position_t prev_buildPos = buildPos;
    int offset = tribunesOffset * num;
    int prev_offset = tribunesOffset * (num + 1);
    buildPos.y += offset;
    prev_buildPos.y += prev_offset;
    if (tableStatus.robot.colorTeam == YELLOW)
        position_robot_flip(buildPos);
   
    nav_return_t nav_ret;
    switch (constructAllTribunesState){
    case FSM_CONSTRUCT_NAV:
        // Nav to the tribune building location (zone)
        // TODO Highways should be enabled
        nav_ret = navigationGoTo(buildPos.x, buildPos.y, buildPos.theta, Direction::SHORTEST, Rotation::SHORTEST, Rotation::SHORTEST, false);
        if (!liftReady)
            liftReady = liftSingleTribune();
        if (nav_ret == NAV_DONE){
            revolverReady = false;
            constructAllTribunesState = FSM_CONSTRUCT_PREPREVOLVER;
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_CONSTRUCT_MOVE:
        nav_ret = navigationGoToNoTurn(prev_buildPos.x - 500, prev_buildPos.y, Direction::SHORTEST, Rotation::SHORTEST, false);
        if (nav_ret == NAV_DONE){
            constructAllTribunesState = FSM_CONSTRUCT_NAV;
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_CONSTRUCT_PREPREVOLVER:
    {
        if (isRevolverEmpty()){
            // TODO Replace the lift down
            constructAllTribunesState = FSM_CONSTRUCT_EXIT;
            return FSM_RETURN_WORKING;
        }
        if (!revolverReady)
            revolverReady = RevolverRelease();
        if (!liftReady)
            liftReady = liftSingleTribune();
        if (revolverReady && liftReady){
            constructAllTribunesState = FSM_CONSTRUCT_BUILD;
            revolverReady = false;
            liftReady = tableStatus.builtTribuneHeights[num] == 2;
        }
        break;
    }
    case FSM_CONSTRUCT_BUILD:
        if (constructSingleTribune()){
            tableStatus.builtTribuneHeights[num]++;
            if (isRevolverEmpty() || tableStatus.robot.plank_count == 0) {
                if (tableStatus.robot.plank_count == 0)
                    LOG_GREEN_INFO("No more planks left, exiting");
                else if (isRevolverEmpty())
                    LOG_GREEN_INFO("No more revolver stock left, exiting");
                constructAllTribunesState = FSM_CONSTRUCT_EXIT;
            }
            else if (tableStatus.builtTribuneHeights[num] == 3){
                LOG_GREEN_INFO("Tribune #", num, " is done");
                constructAllTribunesState = FSM_CONSTRUCT_MOVE;
                num++;
            }
            else {
                LOG_GREEN_INFO("Tribune #", num, " is at height ", tableStatus.builtTribuneHeights[num]);
                constructAllTribunesState = FSM_CONSTRUCT_PREPREVOLVER;
            }
        }
        break;
    case FSM_CONSTRUCT_EXIT:
        // Get out the way to make sure we dont destroy shit
        nav_ret = navigationGoToNoTurn(prev_buildPos.x - 500, prev_buildPos.y, Direction::SHORTEST, Rotation::SHORTEST, false);
        if (nav_ret == NAV_DONE){
            constructAllTribunesState = FSM_CONSTRUCT_NAV;
            return FSM_RETURN_DONE;
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    }
    return FSM_RETURN_WORKING;
}