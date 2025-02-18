#pragma once

typedef enum {
    FROM_LEFT = 0,
    FROM_RIGHT = 1
} direction_t;

// Functions to handle revolver

// initRevolver : Initialize the revolver when the game starts
void initRevolver();
bool isRevolverFull();
bool isRevolverEmpty();

// RevolverPrepareLowBarrel : Prepare the low barrel for incoming stock
bool RevolverPrepareLowBarrel(direction_t dir);
// RevolverLoadStock : Adds the 4 columns of stock to the revolver
bool RevolverLoadStock(direction_t dir);

// RevolverRelease : RevolverRelease the barrel
bool RevolverRelease();

// TestRevolver : Test the revolver
void TestRevolver();