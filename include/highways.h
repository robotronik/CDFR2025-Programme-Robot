#ifndef HIGHWAYS_H
#define HIGHWAYS

#include "constante.h"
#include <limits.h>

#define INF INT_MAX

//Robot width in mm
#define ROBOT_WIDTH 300


typedef struct{
    int x, y;
} highway_point;

typedef struct{
    int a, b;
} highway_line;

typedef enum {
    Circle = 0,
    Square = 1,
    Rectangle = 2
} highway_obstruction_object_type;

typedef struct
{
    highway_point pos;
    bool present;
    int size, size2; //Size2 only use in the rectangle. Describes the radius in mm
    highway_obstruction_object_type type;
} highway_obstruction_object;


// obstruction defs
extern highway_obstruction_object obs_obj_stocks[];
extern highway_obstruction_object obs_obj_opponent;


void init_highways();
int find_fastest_path(highway_point start, highway_point target, highway_point result[]);
bool unit_tests();

#endif