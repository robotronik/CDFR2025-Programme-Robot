#include "SSD1306.hpp"
#include <unistd.h> // For usleep
#include <iostream> // For debugging

SSD1306::SSD1306(int slave_address) : I2CDevice(slave_address) {
    memset(buffer, 0, sizeof(buffer));
    if (i2cFile < 0) {
        std::cerr << "Failed to open I2C device" << std::endl;
    }
}

SSD1306::~SSD1306() {
    if (i2cFile >= 0) {
        close(i2cFile);
    }
}

bool SSD1306::init() {
    if (!testInitSequence()) {
        return false;
    }
    return true;
}

bool SSD1306::testInitSequence() {
    usleep(100000); // Delay for stability

    // Initialize the display with a sequence of commands
    bool success = writeCommand(0xAE) && // Display OFF
                   writeCommand(0xD5) && writeCommand(0x80) && // Set display clock divide ratio/oscillator frequency
                   writeCommand(0xA8) && writeCommand(0x3F) && // Set multiplex ratio(1 to 64)
                   writeCommand(0xD3) && writeCommand(0x00) && // Set display offset. 00 = no offset
                   writeCommand(0x40) && // Set start line address
                   writeCommand(0x8D) && writeCommand(0x14) && // Charge Pump
                   writeCommand(0x20) && writeCommand(0x00) && // Set Memory Addressing Mode
                   writeCommand(0xA1) && // Set segment re-map 0 to 127
                   writeCommand(0xC8) && // Set COM Output Scan Direction
                   writeCommand(0xDA) && writeCommand(0x12) && // Set COM Pins hardware configuration
                   writeCommand(0x81) && writeCommand(0xCF) && // Set contrast control register
                   writeCommand(0xD9) && writeCommand(0xF1) && // Set pre-charge period
                   writeCommand(0xDB) && writeCommand(0x40) && // Set Vcomh deselect level
                   writeCommand(0xA4) && // Entire display ON
                   writeCommand(0xA6) && // Set normal display
                   writeCommand(0x2E) && // Deactivate scroll
                   writeCommand(0xAF); // Display ON

    if (!success) {
        std::cerr << "Failed to initialize the display" << std::endl;
    }

    usleep(100000); // Delay for stability
    return success;
}

void SSD1306::clear() {
    memset(buffer, 0, sizeof(buffer));
}

void SSD1306::display() {
    for (uint8_t i = 0; i < 8; i++) {
        if (!writeCommand(0xB0 + i) || // Set page address
            !writeCommand(0x00) || // Set low column address
            !writeCommand(0x10)) { // Set high column address
            //std::cerr << "Failed to write command to set page address for page " << (int)i << std::endl;
            return;
        }
        if (!writeData(buffer + (i * 128), 128)) {
            //std::cerr << "Failed to write data to page " << (int)i << std::endl;
        }
    }
}

void SSD1306::drawPixel(int x, int y, bool color) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) {
        return;
    }
    if (color) {
        buffer[x + (y / 8) * 128] |= (1 << (y % 8));
    } else {
        buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
    }
}

bool SSD1306::getPixel(int x, int y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) {
        return false;
    }
    return buffer[x + (y / 8) * 128] & (1 << (y % 8));
}

bool SSD1306::writeCommand(uint8_t cmd) {
    uint8_t buffer[2] = {0x00, cmd};
    if (write(i2cFile, buffer, 2) != 2) {
        //std::cerr << "Failed to write command 0x" << std::hex << (int)cmd << " to the i2c bus" << std::endl;
        return false;
    }
    usleep(300); // Short delay after each command for stability
    return true;
}

bool SSD1306::writeData(const uint8_t* data, size_t length) {
    uint8_t buffer[129];
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, length);
    if (write(i2cFile, buffer, length + 1) != length + 1) {
        //std::cerr << "Failed to write data to the i2c bus" << std::endl;
        return false;
    }
    return true;
}

void SSD1306::drawLine(int x0, int y0, int x1, int y1, bool color) {
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



void SSD1306::drawCircle(int x0, int y0, int radius, bool color) {
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

void SSD1306::drawText(int x, int y, const std::string& text, bool color) {
    for (char c : text) {
        drawChar(x, y, c, color);
        x += 6; // Move to the next character position
    }
}

void SSD1306::drawCross(int x, int y, int size, bool color) {
    drawLine(x - size, y, x + size, y, color);
    drawLine(x, y - size, x, y + size, color);
}

void SSD1306::drawChar(int x, int y, char c, bool color) {
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

const uint8_t* SSD1306::getFontData(char c) {
    return font[c - 32];
}


void SSD1306::clearLine(int y) {
    for (int i = 0; i < 128; i++) {
        drawPixel(i, y, false);
        drawPixel(i, y + 1, false);
        drawPixel(i, y + 2, false);
        drawPixel(i, y + 3, false);
        drawPixel(i, y + 4, false);
        drawPixel(i, y + 5, false);
        drawPixel(i, y + 6, false);
        drawPixel(i, y + 7, false);
    }
    display();
}