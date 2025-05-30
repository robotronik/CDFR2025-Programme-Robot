#include <functional> // For std::hash
#include "navigation/navigation.h"
#include "navigation/highways.h"
#include "main.hpp"
#include "defs/constante.h" // DISTANCESTOP and DISTANCESTART
#include "utils/logger.hpp"
#include "lidar/lidarAnalize.h"

#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define HOLD_BACK_DIST 200.0

static bool is_robot_stalled = false;
static bool is_opponent_front = false;
bool override_no_stop = false; // If true, it ignores
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
    if (hashValue == currentInstructionHash && is_robot_stalled && !useHighways){
        if (_millis() < robot_stall_start_time + NAV_MAX_STALL_TIME_MS)
            return NAV_PAUSED;
        // Calculate a new position which is behind the robot
        position_t dir_vector = {(int)(cos(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 
                                (int)(sin(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 0};
        position_t new_pos;
        if (is_opponent_front)
            new_pos = {tableStatus.robot.pos.x - dir_vector.x, tableStatus.robot.pos.y - dir_vector.y, 0};
        else
            new_pos = {tableStatus.robot.pos.x + dir_vector.x, tableStatus.robot.pos.y + dir_vector.y, 0};
        // Clamp the position around the table 20cm
        new_pos.x = clamp(new_pos.x, -1000 + 250, 1000 - 250);
        new_pos.y = clamp(new_pos.y, -1500 + 250, 1500 - 250);
        // Go to the new position
        LOG_WARNING("Robot is stalled, going to new position : ", new_pos.x, " ", new_pos.y);

        asserv.stop();
        asserv.go_to_point(new_pos.x, new_pos.y, Rotation::SHORTEST, Direction::SHORTEST);
        asserv.go_to_point(x,y, theta, rotationLookAt, direction, rotation);
        currentPath[0] = {new_pos.x,new_pos.y};
        currentPath[1] = {x,y};
        currentPathLenght = 2;
        is_robot_stalled = false;
        return (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }
    else if (hashValue == currentInstructionHash && is_robot_stalled && useHighways){
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
                LOG_DEBUG("Going to point with highways : ", x, " ", y);
                if (asserv.get_command_buffer_size() != 0)
                    asserv.stop();
                for (int i = 0; i < currentPathLenght; i++){
                    asserv.go_to_point(currentPath[i].x,currentPath[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
                }
                asserv.consigne_angulaire(theta, rotation);
                currentInstructionHash = hashValue;
            }
        }else{
            LOG_DEBUG("Going to point without highways : ", x, " ", y);
            if (asserv.get_command_buffer_size() != 0)
                asserv.stop();
            asserv.go_to_point(x,y, theta, rotationLookAt, direction, rotation);
            currentPath[0] = {x,y};
            currentPathLenght = 1;
            currentInstructionHash = hashValue;
        }
    }
    else{
        ireturn = (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }
    return ireturn;
}

nav_return_t navigationGoToNoTurn(int x, int y, Direction direction, Rotation rotationLookAt, bool useHighways){
    nav_hash hashValue = createHash(x, y, 0, direction, rotationLookAt, (Rotation)0);
    nav_return_t ireturn = NAV_IN_PROCESS;
    // TODO : Add security for position (ex: outside, scene, opponent protected zones, ...)
    if (hashValue == currentInstructionHash && is_robot_stalled && !useHighways){
        if (_millis() < robot_stall_start_time + NAV_MAX_STALL_TIME_MS)
            return NAV_PAUSED;
        // Calculate a new position which is behind the robot
        position_t dir_vector = {(int)(cos(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 
                                (int)(sin(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 0};
        position_t new_pos;
        if (is_opponent_front)
            new_pos = {tableStatus.robot.pos.x - dir_vector.x, tableStatus.robot.pos.y - dir_vector.y, 0};
        else
            new_pos = {tableStatus.robot.pos.x + dir_vector.x, tableStatus.robot.pos.y + dir_vector.y, 0};
        // Clamp the position around the table 20cm
        new_pos.x = clamp(new_pos.x, -1000 + 250, 1000 - 250);
        new_pos.y = clamp(new_pos.y, -1500 + 250, 1500 - 250);
        // Go to the new position
        LOG_WARNING("Robot is stalled, going to new position : ", new_pos.x, " ", new_pos.y);

        asserv.stop();
        asserv.go_to_point(new_pos.x, new_pos.y, Rotation::SHORTEST, Direction::SHORTEST);
        asserv.go_to_point(x,y, rotationLookAt, direction);
        currentPath[0] = {new_pos.x,new_pos.y};
        currentPath[1] = {x,y};
        currentPathLenght = 2;
        is_robot_stalled = false;
        return (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }
    else if (hashValue == currentInstructionHash && is_robot_stalled && useHighways){
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
                LOG_DEBUG("Going to point with highways : ", x, " ", y);
                if (asserv.get_command_buffer_size() != 0)
                    asserv.stop();
                for (int i = 0; i < currentPathLenght; i++){
                    asserv.go_to_point(currentPath[i].x,currentPath[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
                }
                currentInstructionHash = hashValue;
            }
        }
        else{
            LOG_DEBUG("Going to point without highways : ", x, " ", y);
            if (asserv.get_command_buffer_size() != 0)
                asserv.stop();
            //asserv.set_brake_state(false);
            asserv.go_to_point(x,y, rotationLookAt, direction);
            currentPath[0] = {x,y};
            currentPathLenght = 1;
            currentInstructionHash = hashValue;
        }
    }
    else{
        ireturn = (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }
    return ireturn;
}


nav_return_t navigationPath(position_t path[], int pathLenght, Direction direction, Rotation rotationLookAt, Rotation rotation){
    nav_hash hashValue = createHash(0, 0, 0, direction, rotationLookAt, (Rotation)0);
    for (int i = 0; i < pathLenght; i++){
        hashValue += createHash(path[i].x, path[i].y, 0, direction, rotationLookAt, (Rotation)0);
    }
    nav_return_t ireturn = NAV_IN_PROCESS;
    if (hashValue == currentInstructionHash && is_robot_stalled){
        if (_millis() < robot_stall_start_time + NAV_MAX_STALL_TIME_MS)
            return NAV_PAUSED;
        // Calculate a new position which is behind the robot
        position_t dir_vector = {(int)(cos(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 
                                (int)(sin(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 0};
        position_t new_pos;
        if (is_opponent_front)
            new_pos = {tableStatus.robot.pos.x - dir_vector.x, tableStatus.robot.pos.y - dir_vector.y, 0};
        else
            new_pos = {tableStatus.robot.pos.x + dir_vector.x, tableStatus.robot.pos.y + dir_vector.y, 0};
        // Clamp the position around the table 20cm
        new_pos.x = clamp(new_pos.x, -1000 + 250, 1000 - 250);
        new_pos.y = clamp(new_pos.y, -1500 + 250, 1500 - 250);
        // Go to the new position
        LOG_WARNING("Robot is stalled, going to new position : ", new_pos.x, " ", new_pos.y);

        asserv.stop();
        asserv.go_to_point(new_pos.x, new_pos.y, Rotation::SHORTEST, Direction::SHORTEST);
        currentPath[0] = {new_pos.x,new_pos.y};
        for (int i = 0; i < pathLenght; i++){
            asserv.go_to_point(path[i].x, path[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
            currentPath[i+1] = {path[i].x,path[i].y};
        }
        asserv.consigne_angulaire(path[pathLenght - 1].theta, rotationLookAt);
        currentPathLenght = pathLenght + 1;
        is_robot_stalled = false;
        return (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }

    if (hashValue != currentInstructionHash){
        LOG_DEBUG("Following path");
        if (asserv.get_command_buffer_size() != 0)
            asserv.stop();
        for (int i = 0; i < pathLenght; i++){
            asserv.go_to_point(path[i].x, path[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
            currentPath[i] = {path[i].x,path[i].y};
        }
        asserv.consigne_angulaire(path[pathLenght - 1].theta, rotationLookAt);
        currentPathLenght = pathLenght;
        currentInstructionHash = hashValue;
    }
    else{
        ireturn = (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }
    return ireturn;
}

nav_return_t navigationPathNoTurn(position_t path[], int pathLenght, Direction direction, Rotation rotationLookAt){
    nav_hash hashValue = createHash(0, 0, 0, direction, rotationLookAt, (Rotation)0);
    for (int i = 0; i < pathLenght; i++){
        hashValue += createHash(path[i].x, path[i].y, 0, direction, rotationLookAt, (Rotation)0);
    }
    nav_return_t ireturn = NAV_IN_PROCESS;
    if (hashValue == currentInstructionHash && is_robot_stalled){
        if (_millis() < robot_stall_start_time + NAV_MAX_STALL_TIME_MS)
            return NAV_PAUSED;
        // Calculate a new position which is behind the robot
        position_t dir_vector = {(int)(cos(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 
                                (int)(sin(tableStatus.robot.pos.theta * DEG_TO_RAD) * HOLD_BACK_DIST), 0};
        position_t new_pos;
        if (is_opponent_front)
            new_pos = {tableStatus.robot.pos.x - dir_vector.x, tableStatus.robot.pos.y - dir_vector.y, 0};
        else
            new_pos = {tableStatus.robot.pos.x + dir_vector.x, tableStatus.robot.pos.y + dir_vector.y, 0};
        // Clamp the position around the table 20cm
        new_pos.x = clamp(new_pos.x, -1000 + 250, 1000 - 250);
        new_pos.y = clamp(new_pos.y, -1500 + 250, 1500 - 250);
        // Go to the new position
        LOG_WARNING("Robot is stalled, going to new position : ", new_pos.x, " ", new_pos.y);

        asserv.stop();
        asserv.go_to_point(new_pos.x, new_pos.y, Rotation::SHORTEST, Direction::SHORTEST);
        currentPath[0] = {new_pos.x,new_pos.y};
        for (int i = 0; i < pathLenght; i++){
            asserv.go_to_point(path[i].x, path[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
            currentPath[i+1] = {path[i].x,path[i].y};
        }
        currentPathLenght = pathLenght + 1;
        is_robot_stalled = false;
        return (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
    }

    if (hashValue != currentInstructionHash){
        LOG_DEBUG("Following path");
        if (asserv.get_command_buffer_size() != 0)
            asserv.stop();
        for (int i = 0; i < pathLenght; i++){
            asserv.go_to_point(path[i].x, path[i].y, i == 0 ? rotationLookAt : Rotation::SHORTEST, direction);
            currentPath[i] = {path[i].x,path[i].y};
        }
        currentInstructionHash = hashValue;
        currentPathLenght = pathLenght;
    }
    else{
        ireturn = (asserv.get_moving_is_done() && asserv.get_command_buffer_size() == 0)
                     ? NAV_DONE : NAV_IN_PROCESS;
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

    Direction dir = (Direction)(tableStatus.robot.direction_side);
    bool isEndangered = false;

    if (dir != Direction::NONE){
        // Using the braking distance to calculate a point in front of the robot andh checking if the opponent is in the way
        double brakingDistance = 50 + ROBOT_WIDTH / 2; //tableStatus.robot.braking_distance
        if (dir == Direction::BACKWARD){
            brakingDistance = -brakingDistance;
        }
        // Check if the opponent is in the way
        isEndangered = opponent_collide_lidar(lidar.data, lidar.count, ROBOT_WIDTH, brakingDistance, OPPONENT_ROBOT_RADIUS);
        if (isEndangered)
            LOG_INFO("Opponent is in the way");
        else
            LOG_DEBUG("No opponent in the way");
    }
    // stop the robot if it is endangered
    if (!override_no_stop && isEndangered && !is_robot_stalled){
        LOG_GREEN_INFO("Opponent is in the way, stopping the robot");
        is_opponent_front = dir == Direction::FORWARD;
        asserv.pause();
        //asserv.set_brake_state(true);
        is_robot_stalled = true;
        robot_stall_start_time = _millis();
    }
    else if (!isEndangered && is_robot_stalled){
        LOG_GREEN_INFO("Opponent is no longer in the way, resuming the robot");
        //asserv.set_brake_state(false);
        asserv.resume();
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
