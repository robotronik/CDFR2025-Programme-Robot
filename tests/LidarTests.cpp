#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "lidarAnalize.h"
//#include "lidar.h"
#include "structs.hpp"
#include "constante.h"

bool run_lidar_opponent_test(std::string testName, std::string data_file_name, position_t robot_pos, position_t expected_opponent_pos, bool has_opponent);
bool run_lidar_beacons_test(std::string testName, std::string data_file_name, position_t expected_robot_pos, colorTeam_t expected_color);
int loadLidarJson(std::string filename, lidarAnalize_t* lidarData);

bool test_lidar_opponent() {
    position_t robot_pos = {0, 0, 0, 0, 0};
    position_t expected_opponent_pos = {0, 0, 0, 0, 0};
    // Test 1
    {
        robot_pos.x = 0;
        robot_pos.y = 0;
        robot_pos.theta = 0;
        expected_opponent_pos.x = 340;
        expected_opponent_pos.y = 340;    
        if (!run_lidar_opponent_test("Lidar Opponent Case 1", "lidar/opponent42cm45degLeft.json", robot_pos, expected_opponent_pos, true))
            return false;
    }

    // Test 2
    {
        robot_pos.x = 0;
        robot_pos.y = 0;
        robot_pos.theta = 0;
        expected_opponent_pos.x = 300;
        expected_opponent_pos.y = 0;  
        if (!run_lidar_opponent_test("Lidar Opponent Case 2", "lidar/opponent30cmFront.json", robot_pos, expected_opponent_pos, true))
            return false;
    }

    // Test 3
    // Tests if the code detects that there is no opponent
    {
        robot_pos.x = 0;
        robot_pos.y = 0;
        robot_pos.theta = 0;    
        if (!run_lidar_opponent_test("Lidar Opponent Case 3", "lidar/opponentNone.json", robot_pos, expected_opponent_pos, false))
            return false;
    }

    // Test 4
    {
        robot_pos.x = 0;
        robot_pos.y = -500;
        robot_pos.theta = 90;
        expected_opponent_pos.x = 200;
        expected_opponent_pos.y = 1350;
        if (!run_lidar_opponent_test("Lidar Opponent Case 4", "lidar/opponent190cmFront.json", robot_pos, expected_opponent_pos, true))
            return false;
    }

    // Test 5
    {
        robot_pos.x = 0;
        robot_pos.y = 500;
        robot_pos.theta = 0;
        expected_opponent_pos.x = 0;
        expected_opponent_pos.y = -1500;
        if (!run_lidar_opponent_test("Lidar Opponent Case 5", "lidar/opponent190cm90degRight.json", robot_pos, expected_opponent_pos, true))
            return false;
    }
    return true;
}

bool test_lidar_beacons() {
    // test the transform_coordinates function
    //double x, y, t;
    //transform_coordinates(100, 0, 0, 0, 0, -45, &x, &y, &t);
    //printf("\n\n\npos is %lf , %lf\n\n\n", x, y);
    //return true;


    position_t expected_robot_pos = {0, 0, 0, 0, 0};
    // Test 1
    {
        expected_robot_pos.x = 0;
        expected_robot_pos.y = 0;
        expected_robot_pos.theta = 0;  
        if (!run_lidar_beacons_test("Lidar Beacons Case 1", "lidar/beaconsCenterBlue.json", expected_robot_pos, BLUE));
            //return false;
    }
    // Test 2
    {
        expected_robot_pos.x = -400;
        expected_robot_pos.y = 0;
        expected_robot_pos.theta = 0;  
        if (!run_lidar_beacons_test("Lidar Beacons Case 2", "lidar/beacons40cmUpBlue.json", expected_robot_pos, BLUE))
            return false;
    }

    return true;
}

bool run_lidar_opponent_test(std::string testName, std::string data_file_name, position_t robot_pos, position_t expected_opponent_pos, bool has_opponent) {
    lidarAnalize_t lidarData[SIZEDATALIDAR];
    position_t pos_opponent;
    bool test_passed = false;
    int tolerance = 50; //50mm of tolerance on each axis
    
    // Load the json data into lidarData
    int lidar_count = loadLidarJson(data_file_name, lidarData);
    if (lidar_count < 0)
        return false;

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
    std::cout << testName << (test_passed ? " passed!" : " failed!") << std::endl;
    return test_passed;
}
bool run_lidar_beacons_test(std::string testName, std::string data_file_name, position_t expected_robot_pos, colorTeam_t expected_color) {
    lidarAnalize_t lidarData[SIZEDATALIDAR];
    bool test_passed = false;
    int tolerance = 100; //mm tolerance on each axis
    int angle_tolerance = 15; //degrees of tolerance

    position_t robot_pos;
    colorTeam_t color_team;

    // Load the json data into lidarData
    int lidar_count = loadLidarJson(data_file_name, lidarData);
    if (lidar_count < 0)
        return false;

    //Try method 2
    test_passed = position_robot_beacons(lidarData, lidar_count, &robot_pos, NONE, &color_team);

    //convertAngularToAxial(lidarData, lidar_count, &robot_pos, -100);
    //Try method 1
    //init_position_balise(lidarData, lidar_count, &robot_pos);

    test_passed = test_passed && 
                std::abs(robot_pos.x - expected_robot_pos.x) < tolerance &&
                std::abs(robot_pos.y - expected_robot_pos.y) < tolerance &&
                std::abs(delta_angle(robot_pos.theta, expected_robot_pos.theta)) < angle_tolerance &&
                color_team != NONE && color_team == expected_color;

    std::cout << testName << " Found robot_pos\t= { x=" << robot_pos.x << ",\ty=" << robot_pos.y << ",\ttheta=" << robot_pos.theta <<" }" << std::endl;
    std::cout << testName << " Expects robot_pos\t= { x=" << expected_robot_pos.x << ",\ty=" << expected_robot_pos.y << ",\ttheta=" << expected_robot_pos.theta <<" }" << std::endl;
    std::cout << testName << (test_passed ? " passed!" : " failed!") << std::endl;
    return test_passed;
}

// Returns lidar count
int loadLidarJson(std::string filename, lidarAnalize_t* lidarData) {
    std::ifstream file(filename);
    
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return -1; // Return an error code
    }

    nlohmann::json j;

    // Attempt to parse the JSON file
    try {
        file >> j;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Error: Failed to parse JSON - " << e.what() << std::endl;
        return -1; // Return an error code
    }

    // Ensure "data" is an array
    if (!j.contains("data") || !j["data"].is_array()) {
        std::cerr << "Error: JSON does not contain a valid 'data' array." << std::endl;
        return -1; // Return an error code
    }

    int lidar_count = 0;
    for (const auto& item : j["data"]) {
        if (lidar_count >= SIZEDATALIDAR) {
            std::cerr << "Warning: Maximum lidar data count reached." << std::endl;
            break; // Prevent overflow
        }

        // Check if required fields are present and valid
        if (item.contains("valid")){
            bool valid = item["valid"].get<bool>();
            if (!valid)
                continue;
        }
        if (item.contains("A") && item.contains("R")) {
            lidarAnalize_t point;
            point.angle = item["A"].get<float>();
            point.dist = item["R"].get<float>();
            lidarData[lidar_count] = point;
            lidar_count++;
        } else {
            std::cerr << "Warning: Missing required fields in data item." << std::endl;
        }
    }    
    return lidar_count; // Return the count of loaded lidar data
}
