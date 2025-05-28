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
        ret = DeployBannerFSM(1);
        if (ret == FSM_RETURN_DONE)
            runState = FSM_ACTION_GATHER; // TODO Might need to change
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
    static bool firstTime;
    if (num == -1){
        if (!StratGather(num, offset)){
            LOG_INFO("No more stocks to take, exiting GatherStock");
            num = -1;
            gatherStockState = FSM_GATHER_NAV;
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
    static unsigned long startTime2;
    switch (gatherStockState){
    case FSM_GATHER_NAV:
        // TODO Highways should be enabled for some takes
        nav_ret = navigationGoTo(stockPos.x + stockOff.x, stockPos.y + stockOff.y, stockOff.theta, Direction::SHORTEST, Rotation::SHORTEST, Rotation::SHORTEST, false);
        if (RevolverPrepareLowBarrel(stock_intake_dir) && (nav_ret == NAV_DONE)){
            gatherStockState = FSM_GATHER_MOVE;
            asserv.set_linear_max_speed(160, 300, 300);
            moveClaws(3);
            startTime = _millis();
            LOG_INFO("Nav done and RevolverPrepareLowBarrel done for FSM_GATHER_NAV, going to FSM_GATHER_MOVE");
            firstTime = true;
            override_no_stop = true;
            // TODO Check if theres no opponent before starting the collect
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_GATHER_MOVE:
    {   
        if (readPlankSensors() && firstTime){
            firstTime = false;
            startTime2 = _millis();
        }
        if (_millis() > startTime2 + 350 && !firstTime){
            firstTime = true;
            moveClaws(0);
        }
        if (stockPos.theta == 0) // Horizontal stock
            nav_ret = navigationGoToNoTurn(stockPos.x + stockOff.x, stockPos.y - stockOff.y*0.2, stock_nav_dir, Rotation::SHORTEST, false);
        else // Vertical stock
            nav_ret = navigationGoToNoTurn(stockPos.x - stockOff.x*0.2, stockPos.y + stockOff.y, stock_nav_dir, Rotation::SHORTEST, false);

        bool revolverDone = false;
        if (_millis() > startTime + 750)
            revolverDone = RevolverLoadStock(stock_intake_dir, num);
        if ((nav_ret == NAV_DONE) & revolverDone){
            gatherStockState = FSM_GATHER_COLLECT;
            override_no_stop = false;
            asserv.set_linear_max_speed(10000, 300, 300);
            LOG_INFO("Nav done and revolver done for FSM_GATHER_MOVE, going to FSM_GATHER_COLLECT");
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
            LOG_INFO("taking stock for FSM_GATHER_COLLECT");
            if (!tableStatus.done_banderole && (num == 2 || num == 7)){
                LOG_INFO("Going to deploy banderole");
                runState = FSM_ACTION_DEPLOY_BANNER;
            }

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

    // Offset the build pos by 150mm * num
    const int tribunesOffset = 150;
    position_t buildPos = TRIBUNE_CONSTRUCT_POSITION[zoneNum];
    int offset = tribunesOffset * num;
    buildPos.y += offset;
    if (tableStatus.robot.colorTeam == YELLOW){
        position_robot_flip(buildPos);
    }
    static position_t real_build_pos;
   
    nav_return_t nav_ret;
    switch (constructAllTribunesState){
    case FSM_CONSTRUCT_NAV:
    {
        static unsigned long startTime = _millis();
        // Nav to the tribune building location (zone)
        // TODO Highways should be enabled
        nav_ret = navigationGoTo(buildPos.x, buildPos.y, buildPos.theta, Direction::SHORTEST, Rotation::SHORTEST, Rotation::SHORTEST, false);
        if (!liftReady && (_millis() > startTime + 1000)){
            liftReady = liftSingleTribune();
            movePlatformElevator(3,tableStatus.robot.plank_count*150);
        }
        if (nav_ret == NAV_DONE){
            revolverReady = false;
            constructAllTribunesState = FSM_CONSTRUCT_PREPREVOLVER;
            real_build_pos = tableStatus.robot.pos;
            LOG_INFO("Nav done for FSM_CONSTRUCT_NAV, going to FSM_CONSTRUCT_PREPREVOLVER");
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    }
    case FSM_CONSTRUCT_MOVE:
        
        nav_ret = navigationGoToNoTurn(real_build_pos.x - 400, real_build_pos.y, Direction::SHORTEST, Rotation::SHORTEST, false);
        if (nav_ret == NAV_DONE){
            constructAllTribunesState = FSM_CONSTRUCT_NAV;
            LOG_INFO("Nav done for FSM_CONSTRUCT_MOVE, going to FSM_CONSTRUCT_NAV");
        }
        else if (nav_ret == NAV_ERROR){
            // TODO get another stock
            return FSM_RETURN_ERROR;
        }
        break;
    case FSM_CONSTRUCT_PREPREVOLVER:
    {
        if (!revolverReady && isRevolverEmpty()){
            // Replace the lift down
            if (lowerSingleTribune()){
                constructAllTribunesState = FSM_CONSTRUCT_EXIT;
                LOG_INFO("Revolver Empty for FSM_CONSTRUCT_PREPREVOLVER, going to FSM_CONSTRUCT_EXIT");
            }
            else{
                LOG_INFO("Revolver Empty, lowering single tribune");
            }
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
            LOG_INFO("Revolver ready and lift ready for FSM_CONSTRUCT_PREPREVOLVER, going to FSM_CONSTRUCT_BUILD");
        }
        break;
    }
    case FSM_CONSTRUCT_BUILD:
        if (constructSingleTribuneP(tableStatus.robot.plank_count)){
            tableStatus.builtTribuneHeights[num]++;
            tableStatus.robot.plank_count--;
            LOG_INFO("Building tribune #", num, " for FSM_CONSTRUCT_BUILD");
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
        nav_ret = navigationGoToNoTurn(real_build_pos.x - 400, real_build_pos.y, Direction::SHORTEST, Rotation::SHORTEST, false);
        if (nav_ret == NAV_DONE){
            constructAllTribunesState = FSM_CONSTRUCT_NAV;
            LOG_INFO("Nav done for FSM_CONSTRUCT_EXIT, exiting");
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

// pos  - 0 = middle, 1 = sides
ReturnFSM_t ActionFSM::DeployBannerFSM(int pos){
    nav_return_t nav_ret;
    static bool init = false;
    static position_t init_pos;
    switch (deployBannerState){
    case FSM_DEPLOY_NAV:
        position_t deploy_pos; // Blue pos
        if (!init){
            init_pos = tableStatus.robot.pos;
            init = true;
        }
        if (pos == 0)
            deploy_pos = {800, (tableStatus.robot.colorTeam == YELLOW) ? -400 : 400, (tableStatus.robot.colorTeam == YELLOW) ? 180 : 0}; // TODO
        else if (pos == 1)
            deploy_pos = {800, init_pos.y, (tableStatus.robot.colorTeam == YELLOW) ? 180 : 0};
        
        nav_ret = navigationGoTo(deploy_pos.x, deploy_pos.y, deploy_pos.theta, Direction::SHORTEST, Rotation::SHORTEST, Rotation::SHORTEST);
        if (nav_ret == NAV_DONE){
            deployBannerState = FSM_DEPLOY_EXPL;
            LOG_INFO("Nav done for FSM_DEPLOY_NAV, going to FSM_DEPLOY_EXPL");
        }
        else if (nav_ret == NAV_ERROR)
            return FSM_RETURN_ERROR;
        break;
    case FSM_DEPLOY_EXPL:
        if (deployBanner(tableStatus.robot.colorTeam == BLUE)){
            deployBannerState = FSM_DEPLOY_NAV;
            LOG_INFO("Banner deployed for FSM_DEPLOY_EXPL, done");
            tableStatus.done_banderole = true;
            return FSM_RETURN_DONE;
        }
        break;
    }
    return FSM_RETURN_WORKING;
}