#include <iostream>

bool testDrawPixel();
bool testLogger();
bool test_lidar_opponent();
bool test_lidar_beacons();

int runAllTests();

#define UNIT_TESTS_ONLY

int main() {
    return runAllTests();
}

//Runs every test
int runAllTests() {
    std::cout << "Running tests" << std::endl;
    int numPassed = 0;
    int numTests = 0;

    //Runs the logger tests
    std::cout << "Running logger tests" << std::endl;
    numTests++;
    if(testLogger())
        numPassed++;

#ifndef UNIT_TESTS_ONLY
    //Runs the display tests
    std::cout << "Running display tests" << std::endl;
    numTests++;
    if(testDrawPixel())
        numPassed++;
#endif

    //Runs the lidar tests
    std::cout << "Running lidar tests" << std::endl;
    numTests++;
    if(test_lidar_opponent())
        numPassed++;
    numTests++;
    if(test_lidar_beacons())
        numPassed++;

    std::cout << numPassed << "/" << numTests << " tests passed" << std::endl;
    return (numTests == numPassed) ? 0 : 1;
}