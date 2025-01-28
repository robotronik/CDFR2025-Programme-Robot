#pragma once
#include "utils/utils.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef struct 
{
    int  x;
    int  y;
    int  theta;
}position_t; 

//Define serialization for position_t
void to_json(json& j, const position_t& p);

typedef struct {
    float x;
    float y;
    float angle;
} position_float_t;

typedef struct {
    double x;
    double y;
    double angle;
} position_double_t;

//Define serialization for position_t
void to_json(json& j, const position_double_t& p);

typedef struct 
{
    double angle;
    double dist;
    int    x;
    int    y;
    bool   onTable;
}lidarAnalize_t;

// Define serialization for lidarAnalize_t
void to_json(json& j, const lidarAnalize_t& p);

typedef struct
{
    colorTeam_t colorTeam;// équipe du robot
    position_t pos; //position du robot
    double vit_x,vit_y; //TODO vitesse instantannée du robot
    int columns_count;
    int plank_count;
}robot_t;

// Define serialization for robot_t
void to_json(json& j, const robot_t& p);

double position_distance(position_t p1, position_t p2);