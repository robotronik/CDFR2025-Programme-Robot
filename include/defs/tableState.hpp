#pragma once
#include "defs/structs.hpp"
#include "utils/logger.hpp"
#include "defs/constante.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef struct 
{
    // State is true if action was done or is available
    bool state;
    // Points is the number of points that could be earned from this action
    int points;
}game_element_t;

// Define serialization for game_element_t
void to_json(json& j, const game_element_t& t);

class TableState
{
public:

    TableState();
    ~TableState();

    void reset();
    int getScore();
    void setScore(int score);
    void incrementScore(int score);
    
    /* data generale*/
    position_t pos_opponent;
    unsigned long startTime;
    robot_t robot;
    game_element_t zoneFull[6];          // zone départ/arrivés

    /* data Show Must Go On*/
    game_element_t stocks[STOCK_COUNT];
    game_element_t banderole;

private:
    int score;
};

// Serialize tableState
void to_json(json& j, const TableState& ts);