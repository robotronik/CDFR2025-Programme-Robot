#ifndef HIGHWAYS_H
#define HIGHWAYS

#include "defs/constante.h"
#include <limits.h>
#include <utils/json.hpp>
using json = nlohmann::json;

#define INF INT_MAX

typedef struct{
    int x, y;
} highway_point;

//Define serialization for highway_point
void to_json(json& j, const highway_point& p);

typedef struct{
    int a, b;
} highway_line;

//Define serialization for highway_line
void to_json(json& j, const highway_line& p);

typedef enum {
    Circle = 0,
    Square = 1,
    Rectangle = 2
} highway_obstruction_object_type;

typedef struct
{
    highway_point pos;
    bool present;
    int size, size2; //Size2 only use in the rectangle for height (y). Describes the radius in mm
    highway_obstruction_object_type type;
} highway_obstruction_object;

//Define serialization for highway_obstruction_object
void to_json(json& j, const highway_obstruction_object& p);

void highway_segments_json(json& j);
void highway_obstacles_json(json& j);

// obstruction defs
extern highway_obstruction_object obs_obj_stocks[];
extern highway_obstruction_object obs_obj_opponent;

void init_highways();
int find_fastest_path(highway_point start, highway_point target, highway_point result[]);
bool unit_tests();

#endif