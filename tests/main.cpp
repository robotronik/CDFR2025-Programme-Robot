#include <iostream>
#include "navigation/highways.h"
#include "defs/tableState.hpp"
#include "i2c/Asserv.hpp"
#include "i2c/Arduino.hpp"
#include "utils/logger.hpp"

#define UNIT_TEST(x) numTests++; if(!(x)) {LOG_ERROR("Test failed on line ", __LINE__ );}else { numPassed++;}

bool testLogger();
bool test_lidar_opponent();
bool test_lidar_beacons();

int runAllTests();

int main() {
    return runAllTests();
}

//Runs every test
int runAllTests() {
    LOG_INFO("Running tests");
    int numPassed = 0;
    int numTests = 0;

    //Runs the logger tests
    LOG_INFO("Running logger tests" );
    UNIT_TEST(testLogger());

    LOG_INFO("Running highway tests");
    init_highways();
    UNIT_TEST(unit_tests());

    //Runs the lidar tests
    LOG_INFO("Running lidar tests");
    UNIT_TEST(test_lidar_opponent());
    UNIT_TEST(test_lidar_beacons());

    LOG_INFO("There has been ", numPassed, "/", numTests, " tests passed");
    return (numTests == numPassed) ? 0 : 1;
}