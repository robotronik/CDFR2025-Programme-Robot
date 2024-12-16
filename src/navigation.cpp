#include "navigation.h"

static CmdAsserv* robot;
static TableState* table;
static bool is_robot_stalled;
static unsigned long robot_stall_start_time;

void initNavigation(CmdAsserv* robot_p, TableState* table_p){
    robot = robot_p;
    table = table_p;
    is_robot_stalled = false;
}

nav_return_t navigationGoTo(int x, int y, int theta, CmdAsserv::Direction direction,CmdAsserv::Rotation rotationLookAt,CmdAsserv::Rotation rotation){
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (is_robot_stalled){
        ireturn = (_millis() > robot_stall_start_time + NAV_MAX_STALL_TIME_MS) ? NAV_PAUSED : NAV_ERROR;
        return ireturn;
    }

    robot->go_to_point(x,y, theta, rotationLookAt, direction, rotation);
    ireturn = robot->get_moving_is_done() ? NAV_DONE : NAV_IN_PROCESS;
    return ireturn;
}

nav_return_t navigationGoToNoTurn(int x, int y, CmdAsserv::Direction direction,CmdAsserv::Rotation rotationLookAt){
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (is_robot_stalled){
        ireturn = (_millis() > robot_stall_start_time + NAV_MAX_STALL_TIME_MS) ? NAV_PAUSED : NAV_ERROR;
        return ireturn;
    }

    robot->go_to_point(x,y, direction, rotationLookAt);
    ireturn = robot->get_moving_is_done() ? NAV_DONE : NAV_IN_PROCESS;
    return ireturn;
}

void navigationOpponentDetection(){
    // This is where it disables the robot in case it might collide with the opponent
    // Called from main, start of main fsm

    bool isEndangered = false;
    switch (robot->get_direction_side())
    {
    case CmdAsserv::FORWARD:
        isEndangered = robot->get_braking_distance() > lidar.collideDistanceFORWARD();
        break;        
    case CmdAsserv::BACKWARD:
        isEndangered = robot->get_braking_distance() > lidar.collideDistanceBACKWARD();
        break;    
    case CmdAsserv::NONE:
        isEndangered = false;
        break;
    default:
        break;
    }

    if(isEndangered && !is_robot_stalled){
        robot.pause();
        is_robot_stalled = true;
        robot_stall_start_time = _millis();
    }
    else if(is_robot_stalled){
        robot.resume();
        is_robot_stalled = false;
    }
}