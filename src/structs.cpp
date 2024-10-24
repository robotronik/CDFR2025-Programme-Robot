#include "structs.hpp"

//Define serialization for position_t
void to_json(json& j, const position_t& p) {
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"dist", p.dist},
        {"theta", p.theta},
        {"time", p.time}
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
        {"collide", p.collide},
        {"columns_count", p.columns_count},
        {"plank_count", p.plank_count}
    };
}

double position_distance(position_t p1, position_t p2){
    // Calculates the position between the two points
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}