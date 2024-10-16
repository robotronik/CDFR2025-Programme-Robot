#include "structs.hpp"

//Define serialization for position_t
void to_json(json& j, const position_t& p) {
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"dist", p.dist},
        {"teta", p.teta},
        {"time", p.time}
    };
}

// Define serialization for lidarAnalize_t
void to_json(json& j, const lidarAnalize_t& p) {
    j = json{
        {"valid", p.valid},
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