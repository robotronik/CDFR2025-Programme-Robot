#include "affichage.hpp"

bool testDrawPixel() {
    Affichage affichage(0x3C);
    affichage.testInitSequence();
    affichage.clear();
    affichage.display();


    affichage.drawText(0, 0, "Hello, world! 198", true);
    affichage.drawText(0, 8, "Ludo Is Gay", true);

    affichage.display();
    sleep(5);

    return true;
}


int runAllTests() {
    std::cout << "Running all tests" << std::endl;
    int numPassed = 0;
    int numTests = 0;
    if(testDrawPixel()) numPassed++; numTests++;

    std::cout << numPassed << "/" << numTests << " tests passed" << std::endl;
    return (numTests == numPassed) ? 0 : 1;
}
