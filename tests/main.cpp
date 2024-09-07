#include <iostream>

bool testDrawPixel();
bool testLogger();

int runAllTests();

int main() {
    return runAllTests();
}

//Runs every test
int runAllTests() {
    std::cout << "Running all tests" << std::endl;
    int numPassed = 0;
    int numTests = 0;

    //Runs the logger tests
    std::cout << "Running logger tests" << std::endl;
    numTests++;
    if(testLogger())
        numPassed++;

    //Runs the display tests
    std::cout << "Running display tests" << std::endl;
    numTests++;
    if(testDrawPixel())
        numPassed++;

    std::cout << numPassed << "/" << numTests << " tests passed" << std::endl;
    return (numTests == numPassed) ? 0 : 1;
}