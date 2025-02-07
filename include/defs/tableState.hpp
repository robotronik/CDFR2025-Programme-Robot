#pragma once
#include "defs/structs.hpp"
#include "utils/logger.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef struct 
{
    bool etat; // si l'action a été faite ou non
    int cout; // points que rapporte l'action
    float tps; //temps passé par l'opponent dans l'action
    colorTeam_t color; //NONE, BLUE or YELLOW
}table_t;

// Define serialization for table_t
void to_json(json& j, const table_t& t);

class TableState
{
public:

    TableState();
    ~TableState();
    void init();

    int getScore();
    void setScore(int score);
    void incrementScore(int score);
    
    /* data generale*/
    position_t pos_opponent;
    unsigned long startTime;
    bool FIN;
    robot_t robot;
    table_t zoneFull[6];          // zone départ/arrivés

    /* data Show Must Go On*/
    table_t stock[10];
    table_t banderole;

private:
    int score;
};

// Serialize tableState
void to_json(json& j, const TableState& ts);