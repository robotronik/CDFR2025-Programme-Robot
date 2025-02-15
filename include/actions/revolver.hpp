#pragma once

typedef enum {
    FROM_LEFT = 0,
    FROM_RIGHT = 1
} direction_t;

// Functions to handle revolver
// Take : Take a stock from a direction
void take(direction_t dir);
// Release : Release the barrel
bool Release();
// TestRevolver : Test the revolver
void TestRevolver();