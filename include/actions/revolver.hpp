#pragma once

typedef enum {
    FROM_LEFT = 0,
    FROM_RIGHT = 1
} direction_t;

// Functions to handle revolver

// initRevolver : Initialize the revolver when the game starts
void initRevolver();

// PrepareLowBarrel : Prepare the low barrel for incoming stock
bool PrepareLowBarrel(direction_t dir);
// LoadStock : Take a stock and place it in the revolver
bool LoadStock(direction_t dir);

// Release : Release the barrel
bool Release();

// TestRevolver : Test the revolver
void TestRevolver();