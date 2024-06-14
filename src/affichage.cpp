#include "affichage.hpp"

// Simple font (5x7) for demonstration purposes
const uint8_t font[96][5] = {
    // ASCII 32 to 127
    // Each character is 5x7 pixels
    // Define your font here, this is just a placeholder
};

Affichage::Affichage(int slave_address) : SSD1306(slave_address) {}

void Affichage::drawLine(int x0, int y0, int x1, int y1, bool color) {
    // Bresenham's line algorithm
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void Affichage::drawCircle(int x0, int y0, int radius, bool color) {
    // Midpoint circle algorithm
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    drawPixel(x0, y0 + radius, color);
    drawPixel(x0, y0 - radius, color);
    drawPixel(x0 + radius, y0, color);
    drawPixel(x0 - radius, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void Affichage::drawText(int x, int y, const std::string& text, bool color) {
    for (char c : text) {
        drawChar(x, y, c, color);
        x += 6; // Move to the next character position
    }
}

void Affichage::drawCross(int x, int y, int size, bool color) {
    drawLine(x - size, y, x + size, y, color);
    drawLine(x, y - size, x, y + size, color);
}

void Affichage::drawChar(int x, int y, char c, bool color) {
    if (c < 32 || c > 127) return; // Character out of font range
    const uint8_t* data = getFontData(c);
    for (int i = 0; i < 5; i++) {
        uint8_t line = data[i];
        for (int j = 0; j < 8; j++) {
            if (line & 0x1) {
                drawPixel(x + i, y + j, color);
            } else {
                drawPixel(x + i, y + j, !color);
            }
            line >>= 1;
        }
    }
}

const uint8_t* Affichage::getFontData(char c) {
    return font[c - 32];
}
