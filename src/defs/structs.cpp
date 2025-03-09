#include "defs/structs.hpp"
#include <math.h>


// Define a function to calculate the angle in rads from and to a position_t
double position_angle(position_t from, position_t to){
    // Calculates the angle between the two points
    return atan2(to.y - from.y, to.x - from.x);
}

// Define a function to calculate the distance from and to a position_t
double position_distance(position_t from, position_t to){
    // Calculates the position between the two points
    return sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));
}


//Define serialization for position_t
void to_json(json& j, const position_t& p) {
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"theta", p.theta}
    };
}

//Define serialization for position_t
void to_json(json& j, const position_double_t& p){
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"angle", p.angle},
    };
}

// Define serialization for lidarAnalize_t
void to_json(json& j, const lidarAnalize_t& p) {
    j = json{
        {"A", p.angle},
        {"R", p.dist},
        //{"x", p.x},
        //{"y", p.y},
        //{"onTable", p.onTable}
    };
}

// Define serialization for robot_t
void to_json(json& j, const robot_t& p) {
    j = json{
        {"colorTeam", p.colorTeam},
        {"pos", p.pos},
        {"vit_x", p.vit_x},
        {"vit_y", p.vit_y},
        {"plank_count", p.plank_count},
        {"target", p.target},
        {"braking_distance", p.braking_distance},
        {"direction_side", p.direction_side},
    };
}

double position_distance(position_t p1, position_t p2){
    // Calculates the position between the two points
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}