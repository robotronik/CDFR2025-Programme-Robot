#pragma once
#include "fonction.h"
#define NOMBRE_TEST_Angle 12     //12 angles 
#define NOMBRE_TEST_DROIT 4

int getOrientation(int x1, int y1, int x2, int y2);

void RobotAdverse_obstacle(tableState* itable, float pastemps, int numpas );

void RobotActualise(robot_t* robot, int x , int y, int time);

float Euristique(position_t current, position_t objectif,int tetaMouv, int MouvDist);

int RobotGetZone(position_t pos, int N);



instruction_t* A_star(tableState* itable, position_t objectif); 