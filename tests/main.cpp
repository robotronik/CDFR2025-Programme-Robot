#include <iostream>

bool testDrawPixel();

int runAllTests();

int main() {
    return runAllTests();
}

//Runs every test
int runAllTests() {
    std::cout << "Running all tests" << std::endl;
    int numPassed = 0;
    int numTests = 0;

    //Runs a test
    numTests++;
    if(testDrawPixel())
        numPassed++;

    std::cout << numPassed << "/" << numTests << " tests passed" << std::endl;
    return (numTests == numPassed) ? 0 : 1;
}