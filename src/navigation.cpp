#include <functional> // For std::hash
#include "navigation.h"
#include "main.hpp"
#include "constante.h" // DISTANCESTOP and DISTANCESTART

static bool is_robot_stalled;
static unsigned long robot_stall_start_time;
typedef std::size_t nav_hash;
static nav_hash currentInstructionHash;

nav_hash createHash(int x, int y, int theta, Direction direction, Rotation rotationLookAt, Rotation rotation);

nav_return_t navigationGoTo(int x, int y, int theta, Direction direction, Rotation rotationLookAt, Rotation rotation){
    nav_hash hashValue = createHash(x, y, theta, direction, rotationLookAt, rotation);
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (is_robot_stalled){
        ireturn = (_millis() > robot_stall_start_time + NAV_MAX_STALL_TIME_MS) ? NAV_PAUSED : NAV_ERROR;
        return ireturn;
    }
    if (hashValue != currentInstructionHash){
        robotI2C.stop();
        robotI2C.go_to_point(x,y, theta, rotationLookAt, direction, rotation);
        currentInstructionHash = hashValue;
    }
    ireturn = robotI2C.get_moving_is_done() ? NAV_DONE : NAV_IN_PROCESS;
    return ireturn;
}

nav_return_t navigationGoToNoTurn(int x, int y, Direction direction, Rotation rotationLookAt){
    nav_hash hashValue = createHash(x, y, 0, direction, rotationLookAt, (Rotation)0);
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (is_robot_stalled){
        ireturn = (_millis() > robot_stall_start_time + NAV_MAX_STALL_TIME_MS) ? NAV_PAUSED : NAV_ERROR;
        return ireturn;
    }

    if (hashValue != currentInstructionHash){
        robotI2C.stop();
        robotI2C.go_to_point(x,y, rotationLookAt, direction);
        currentInstructionHash = hashValue;
    }
    ireturn = robotI2C.get_moving_is_done() ? NAV_DONE : NAV_IN_PROCESS;
    return ireturn;
}

void navigationOpponentDetection(){
    // This is where it disables the robot in case it might collide with the opponent
    // Called from main, start of main fsm

    bool isEndangered = false;
    switch (robotI2C.get_direction_side())
    {
    case Direction::FORWARD:
        isEndangered = robotI2C.get_braking_distance() > 69; // TODO find some value... > lidar.collideDistanceFORWARD();
        break;        
    case Direction::BACKWARD:
        isEndangered = robotI2C.get_braking_distance() > 69;// > lidar.collideDistanceBACKWARD();
        break;    
    case Direction::NONE:
        isEndangered = false;
        break;
    default:
        break;
    }
    return; // TODO TODO REMOVE WHEN YOU HAVE LIDAR

    //TODO : Maybe call Collide ?

    if(isEndangered && !is_robot_stalled){
        robotI2C.pause();
        is_robot_stalled = true;
        robot_stall_start_time = _millis();
    }
    else if(is_robot_stalled){
        robotI2C.resume();
        is_robot_stalled = false;
    }
}


nav_hash createHash(int x, int y, int theta, Direction direction, Rotation rotationLookAt, Rotation rotation) {
    // Combine all values simply by adding their hashes
    return std::hash<int>{}(x) ^
           std::hash<int>{}(y) ^
           std::hash<int>{}(theta) ^
           std::hash<int>{}(static_cast<int>(direction)) ^
           std::hash<int>{}(static_cast<int>(rotationLookAt)) ^
           std::hash<int>{}(static_cast<int>(rotation));
}