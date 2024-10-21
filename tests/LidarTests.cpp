#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "lidarAnalize.h"
//#include "lidar.h"
#include "structs.hpp"
#include "constante.h"

bool run_lidar_opponent_test(const std::string& testName, const std::string& data_file_name, position_t robot_pos, position_t expected_opponent_pos, bool has_opponent);
bool run_lidar_beacons_test(const std::string& testName, const std::string& data_file_name, position_t approx_robot_pos, position_t expected_robot_pos);
int loadLidarJson(const std::string& filename, lidarAnalize_t* lidarData);

bool test_lidar_opponent() {
    position_t robot_pos = {0, 0, 0, 0, 0};
    position_t expected_opponent_pos = {0, 0, 0, 0, 0};
    // Test 1
    {
        robot_pos.x = 0;
        robot_pos.y = 0;
        robot_pos.theta = 0;
        expected_opponent_pos.x = 1100;
        expected_opponent_pos.y = 550;    
        if (!run_lidar_opponent_test("Lidar Opponent Case 1", "data1.json", robot_pos, expected_opponent_pos, true))
            return false;
    }

    // Test 2
    {
        robot_pos.x = 500;
        robot_pos.y = -500;
        robot_pos.theta = 45;
        expected_opponent_pos.x = 1100;
        expected_opponent_pos.y = 550;  
        if (!run_lidar_opponent_test("Lidar Opponent Case 2", "data2.json", robot_pos, expected_opponent_pos, true))
            return false;
    }

    // Test 3
    // Tests if the code detects that there is no opponent
    {
        robot_pos.x = 500;
        robot_pos.y = -500;
        robot_pos.theta = 45;    
        if (!run_lidar_opponent_test("Lidar Opponent Case 3", "data3.json", robot_pos, expected_opponent_pos, false))
            return false;
    }

    return true;
}

bool test_lidar_beacons() {
    position_t approx_robot_pos = {0, 0, 0, 0, 0};
    position_t expected_robot_pos = {0, 0, 0, 0, 0};
    // Test 1
    {
        approx_robot_pos.x = 500;
        approx_robot_pos.y = -500;
        approx_robot_pos.theta = 45;  
        expected_robot_pos.x = 512;
        expected_robot_pos.y = -508;
        expected_robot_pos.theta = 47;  
        if (!run_lidar_beacons_test("Lidar Beacons Case 1", "data1.json", approx_robot_pos, expected_robot_pos))
            return false;
    }

    return true;
}

bool run_lidar_opponent_test(const std::string& testName, const std::string& data_file_name, position_t robot_pos, position_t expected_opponent_pos, bool has_opponent) {
    lidarAnalize_t lidarData[SIZEDATALIDAR];
    position_t pos_opponent;
    bool test_passed = false;
    int tolerance = 20; //20mm of tolerance on each axis
    
    // Load the json data into lidarData
    int lidar_count = loadLidarJson(data_file_name, lidarData);
    convertAngularToAxial(lidarData, lidar_count, &robot_pos, 50);

    if (position_opponentV2(lidarData, lidar_count, robot_pos, &pos_opponent)){
        std::cout << testName << " Found opponent_pos\t= {\tx=" << pos_opponent.x << ",\ty=" << pos_opponent.y << " }" << std::endl;
        std::cout << testName << " Expects opponent_pos\t= {\tx=" << expected_opponent_pos.x << ",\ty=" << expected_opponent_pos.y << " }" << std::endl;
        test_passed = std::abs(pos_opponent.x - expected_opponent_pos.x) < tolerance &&
                    std::abs(pos_opponent.y - expected_opponent_pos.y) < tolerance;
    }
    else{        
        std::cout << testName << " Did not find any opponent" << std::endl;
        test_passed = !has_opponent;
    }

    if (test_passed) {
        std::cout << testName << " passed!" << std::endl;
    } else {
        std::cout << testName << " failed!" << std::endl;
    }
    return test_passed;
}
bool run_lidar_beacons_test(const std::string& testName, const std::string& data_file_name, position_t approx_robot_pos, position_t expected_robot_pos) {
    lidarAnalize_t lidarData[SIZEDATALIDAR];
    bool test_passed = false;
    int tolerance = 10; //10mm of tolerance on each axis
    int angle_tolerance = 5; //5 degrees of tolerance

    position_t robot_pos = approx_robot_pos;

    // Load the json data into lidarData
    int lidar_count = loadLidarJson(data_file_name, lidarData);

    convertAngularToAxial(lidarData, lidar_count, &robot_pos, -100);
    init_position_balise(lidarData, lidar_count, &robot_pos);

    test_passed = std::abs(robot_pos.x - expected_robot_pos.x) < tolerance &&
                    std::abs(robot_pos.y - expected_robot_pos.y) < tolerance &&
                    std::abs(robot_pos.theta - expected_robot_pos.theta) < angle_tolerance;

    std::cout << testName << " Found robot_pos\t= { x=" << robot_pos.x << ",\ty=" << robot_pos.y << ",\ttheta=" << robot_pos.theta <<" }" << std::endl;
    std::cout << testName << " Expects robot_pos\t= { x=" << expected_robot_pos.x << ",\ty=" << expected_robot_pos.y << ",\ttheta=" << expected_robot_pos.theta <<" }" << std::endl;

    if (test_passed) {
        std::cout << testName << " passed!" << std::endl;
    } else {
        std::cout << testName << " failed!" << std::endl;
    }
    return test_passed;
}

// Returns lidar count
int loadLidarJson(const std::string& filename, lidarAnalize_t* lidarData) {
    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;
    
    int lidar_count = 0;
    for (const auto& item : j["data"]) {
        lidarAnalize_t point;
        point.valid = item["valid"];
        point.angle = item["A"];
        point.dist = item["R"];
        lidarData[lidar_count] = point;
        lidar_count++;
    }
    return 0;
}
