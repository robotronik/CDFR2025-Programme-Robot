#pragma once

#include "SSD1306.hpp"
#include <string>

class Affichage {
public:
    Affichage(SSD1306& display);
    void setTeamColor(bool color);
    void updateScore(int score);
    void updatePosition(int x, int y);
    void showTeamName(const std::string& name);

private:
    SSD1306& display;
    std::string teamColor;
    int score;
    std::string teamName;
};
