#pragma once

#include "SSD1306.hpp"
#include <string>

class Affichage : public SSD1306 {
public:
    Affichage(int slave_address);
    void drawLine(int x0, int y0, int x1, int y1, bool color);
    void drawCircle(int x0, int y0, int radius, bool color);
    void drawText(int x, int y, const std::string& text, bool color);
    void drawCross(int x, int y, int size, bool color);

private:
    void drawChar(int x, int y, char c, bool color);
    const uint8_t* getFontData(char c);
};
