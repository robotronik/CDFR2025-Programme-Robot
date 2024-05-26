#pragma once
#include "fonction.h"
#define NOMBRE_TEST_Angle 12     //12 angles 
#define NOMBRE_TEST_DROIT 4

struct CompareInstruction{
    bool operator()(const instruction_t& a, const instruction_t& b){
        return a.cout > b.cout;
    }
};

typedef std::priority_queue<instruction_t,std::vector<instruction_t>, CompareInstruction> heap;
typedef std::stack<instruction_t> Astack;

instruction_t stack_pop(Astack* stack, position_t* parent);


float distance2(position_t p1, position_t p2);

int getOrientation(int x1, int y1, int x2, int y2);

void RobotAdverse_obstacle(tableState* itable, float pastemps, int numpas );

void RobotActualise(robot_t* robot, int x , int y, int time);

float Euristique(position_t current, position_t objectif,int tetaMouv, int MouvDist);

int RobotGetZone(position_t pos, int N);

void CalculeNext(heap* Calculer, position_t objectif, position_t currentPos,position_t nextPos);

instruction_t* A_star(tableState* itable, position_t objectif); 