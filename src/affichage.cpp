#include "affichage.hpp"

Affichage::Affichage(SSD1306& display) : display(display), teamColor("BLUE"), teamName("Robotronik"), score(0) {}

void Affichage::setTeamColor(bool color) {
    if(color == true) {
        teamColor = "BLUE";
    } else {
        teamColor = "RED";
    }
    display.clearLine(48);
    display.drawText(0, 48, "Team: " + teamColor, true);
    display.display();
}

void Affichage::updateScore(int newScore) {
    score = newScore;
    display.clearLine(16);
    display.drawText(0, 16, "Score: " + std::to_string(score), true);
    display.display();
}

void Affichage::showTeamName(const std::string& name) {
    teamName = name;
    display.clearLine(0);
    display.drawText((128 - name.length() * 6) / 2, 0, name, true);
    display.display();
}

void Affichage::updatePosition(int x, int y) {
    display.clearLine(32);
    display.drawText(0, 32, "Position: (" + std::to_string(x) + ", " + std::to_string(y) + ")", true);
    display.display();
}


