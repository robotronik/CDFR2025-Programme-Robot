#include "affichage.hpp"

bool testDrawPixel() {
    SSD1306 display(0x3C);
    display.init();
    Affichage affichage(display);
    affichage.setTeamColor(true);
    affichage.updateScore(42);
    affichage.showTeamName("Robotronik");
    affichage.updatePosition(1, 2);
    sleep(5);
    // Update score & position to test in for loop
    for(int i = 0; i < 10; i++) {
        affichage.updateScore(i);
        affichage.updatePosition(i, i);
        sleep(1);
    }

    return true;
}