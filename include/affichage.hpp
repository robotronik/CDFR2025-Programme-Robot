#pragma once

#include "SSD1306.hpp"
#include <string>

class Affichage {
public:
    Affichage(SSD1306& display);
    void init();

    /**
     * Set the team color to display
     * @param color true for blue, false for red
     */
    void setTeamColor(bool color);

    /**
     * Update the score to display
     * @param score the new score
     */
    void updateScore(int score);
    /**
     * Update the position to display
     * @param x X coordinate
     */
    void updatePosition(int x, int y);

    /**
     * Show the team name on the display
     * @param name the team name
     */
    void showTeamName(const std::string& name);

private:
    SSD1306& display;
    std::string teamColor;
    std::string teamName;
};
