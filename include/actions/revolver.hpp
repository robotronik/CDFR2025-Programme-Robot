#pragma once
#include <stdbool.h>

#define SIZE 14
void DisplayRobot();
void DisplayBarrel();
bool SpinBarrel(int n, int num_tab);
bool MoveColumns(int direction, int sens);
bool PrerareLowBarrel(int sens);
bool PrepareHighBarrel(int sens);

bool LoadStock(int direction);
void take(int sens);
void TestRevolver();