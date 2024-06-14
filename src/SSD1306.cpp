#include "SSD1306.hpp"

SSD1306::SSD1306(int slave_address) : I2CDevice(slave_address) {
    memset(buffer, 0, sizeof(buffer));
}

SSD1306::~SSD1306() {
    if (i2cFile >= 0) {
        close(i2cFile);
    }
}

bool SSD1306::init() {
    // Initialize the display with a sequence of commands
    return writeCommand(0xAE) && // Display OFF
           writeCommand(0x20) && writeCommand(0x00) && // Set Memory Addressing Mode
           writeCommand(0xB0) && // Set Page Start Address for Page Addressing Mode
           writeCommand(0xC8) && // COM Output Scan Direction
           writeCommand(0x00) && writeCommand(0x10) && // Set low and high column address
           writeCommand(0x40) && // Set start line address
           writeCommand(0x81) && writeCommand(0xFF) && // Set contrast control
           writeCommand(0xA1) && // Set segment re-map 0 to 127
           writeCommand(0xA6) && // Set normal display
           writeCommand(0xA8) && writeCommand(0x3F) && // Set multiplex ratio
           writeCommand(0xA4) && // Output RAM to display
           writeCommand(0xD3) && writeCommand(0x00) && // Set display offset
           writeCommand(0xD5) && writeCommand(0xF0) && // Set display clock divide ratio/oscillator frequency
           writeCommand(0xD9) && writeCommand(0x22) && // Set pre-charge period
           writeCommand(0xDA) && writeCommand(0x12) && // Set com pins hardware configuration
           writeCommand(0xDB) && writeCommand(0x20) && // Set Vcomh deselect level
           writeCommand(0x8D) && writeCommand(0x14) && // Charge pump
           writeCommand(0xAF); // Display ON
}

void SSD1306::clear() {
    memset(buffer, 0, sizeof(buffer));
}

void SSD1306::display() {
    for (uint8_t i = 0; i < 8; i++) {
        writeCommand(0xB0 + i); // Set page address
        writeCommand(0x00); // Set low column address
        writeCommand(0x10); // Set high column address
        writeData(buffer + (i * 128), 128);
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

bool SSD1306::writeCommand(uint8_t cmd) {
    uint8_t buffer[2] = {0x00, cmd};
    if (write(i2cFile, buffer, 2) != 2) {
        std::cerr << "Failed to write command to the i2c bus" << std::endl;
        return false;
    }
    return true;
}

bool SSD1306::writeData(const uint8_t* data, size_t length) {
    uint8_t buffer[129];
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, length);
    if (write(i2cFile, buffer, length + 1) != length + 1) {
        std::cerr << "Failed to write data to the i2c bus" << std::endl;
        return false;
    }
    return true;
}
