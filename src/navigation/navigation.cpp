#include <functional> // For std::hash
#include "navigation/navigation.h"
#include "navigation/highways.h"
#include "main.hpp"
#include "defs/constante.h" // DISTANCESTOP and DISTANCESTART

static bool is_robot_stalled;
static unsigned long robot_stall_start_time;
typedef std::size_t nav_hash;
static nav_hash currentInstructionHash;

static highway_point currentPath[512];
static int currentPathLenght = 0;

nav_hash createHash(int x, int y, int theta, Direction direction, Rotation rotationLookAt, Rotation rotation);

nav_return_t navigationGoTo(int x, int y, int theta, Direction direction, Rotation rotationLookAt, Rotation rotation, bool useHighways){
    nav_hash hashValue = createHash(x, y, theta, direction, rotationLookAt, rotation);
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (is_robot_stalled && !useHighways){
        ireturn = (_millis() > robot_stall_start_time + NAV_MAX_STALL_TIME_MS) ? NAV_PAUSED : NAV_ERROR;
        return ireturn;
    }
    else if (is_robot_stalled && useHighways){
        LOG_WARNING("Robot is stalled, but using highways to recalculate path");
        currentInstructionHash = 0; // Reset the hash to recalculate the path
    }
    if (hashValue != currentInstructionHash){
        if (useHighways){
            highway_point start = {tableStatus.robot.pos.x, tableStatus.robot.pos.y};
            highway_point target = {x,y};
            currentPathLenght = find_fastest_path(start, target, currentPath);
            if (currentPathLenght == 0){ //Error
                LOG_ERROR("Could not find path to target");
                return NAV_ERROR;
            }else{
                robotI2C.stop();
                for (int i = 0; i < currentPathLenght; i++){
                    robotI2C.go_to_point(currentPath[i].x,currentPath[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
                }
                robotI2C.consigne_angulaire(theta, rotation);
                currentInstructionHash = hashValue;
            }
        }else{
            robotI2C.stop();
            robotI2C.go_to_point(x,y, theta, rotationLookAt, direction, rotation);
            currentPath[0] = {x,y};
            currentPathLenght = 1;
            currentInstructionHash = hashValue;
        }
    }
    else{
        ireturn = robotI2C.get_moving_is_done() ? NAV_DONE : NAV_IN_PROCESS;
    }
    return ireturn;
}

nav_return_t navigationGoToNoTurn(int x, int y, Direction direction, Rotation rotationLookAt, bool useHighways){
    nav_hash hashValue = createHash(x, y, 0, direction, rotationLookAt, (Rotation)0);
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (is_robot_stalled && !useHighways){
        ireturn = (_millis() > robot_stall_start_time + NAV_MAX_STALL_TIME_MS) ? NAV_PAUSED : NAV_ERROR;
        return ireturn;
    }
    else if (is_robot_stalled && useHighways){
        LOG_WARNING("Robot is stalled, but using highways to recalculate path");
        currentInstructionHash = 0; // Reset the hash to recalculate the path
    }

    if (hashValue != currentInstructionHash){
        if (useHighways){
            highway_point start = {tableStatus.robot.pos.x, tableStatus.robot.pos.y};
            highway_point target = {x,y};
            currentPathLenght = find_fastest_path(start, target, currentPath);
            LOG_GREEN_INFO("Path length : ", currentPathLenght);
            if (currentPathLenght == 0){ //Error
                LOG_ERROR("Could not find path to target");
                return NAV_ERROR;
            }else{
                robotI2C.stop();
                for (int i = 0; i < currentPathLenght; i++){
                    robotI2C.go_to_point(currentPath[i].x,currentPath[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
                }
                currentInstructionHash = hashValue;
            }
        }
        else{
            robotI2C.stop();
            robotI2C.go_to_point(x,y, rotationLookAt, direction);
            currentPath[0] = {x,y};
            currentPathLenght = 1;
            currentInstructionHash = hashValue;
        }
    }
    else{
        ireturn = robotI2C.get_moving_is_done() ? NAV_DONE : NAV_IN_PROCESS;
    }
    return ireturn;
}

void navigation_path_json(json& j){
    j = json::array();
    j.push_back({{"x", tableStatus.robot.pos.x}, {"y", tableStatus.robot.pos.y}});
    for (int i = 0; i < currentPathLenght; i++){
        j.push_back({{"x", currentPath[i].x}, {"y", currentPath[i].y}});
    }
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
    else if(!isEndangered && is_robot_stalled){
        robotI2C.resume();
        is_robot_stalled = false;
    }
}

// Function to calculate the hash of the navigation instruction
nav_hash createHash(int x, int y, int theta, Direction direction, Rotation rotationLookAt, Rotation rotation) {
    // Combine all values simply by adding their hashes
    return std::hash<int>{}(x) ^
           std::hash<int>{}(y) ^
           std::hash<int>{}(theta) ^
           std::hash<int>{}(static_cast<int>(direction)) ^
           std::hash<int>{}(static_cast<int>(rotationLookAt)) ^
           std::hash<int>{}(static_cast<int>(rotation));
}