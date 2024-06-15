#pragma once

#include "I2CDevice.hpp"
#include <cstring>
#include <iostream>

class SSD1306 : public I2CDevice {
public:
    SSD1306(int slave_address);
    ~SSD1306();
    bool init();
    void clear();
    void display();
    void drawPixel(int x, int y, bool color);
    bool getPixel(int x, int y);
    bool testInitSequence();
    int testSequence();

private:
    uint8_t buffer[1024]; // Buffer for 128x64 display

    bool writeCommand(uint8_t cmd);
    bool writeData(const uint8_t* data, size_t length);
};
