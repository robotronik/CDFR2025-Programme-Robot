#pragma once
#include "defs/structs.hpp"
#include "defs/constante.h"
#include <utils/json.hpp>
using json = nlohmann::json;

class TableState
{
public:

    TableState();
    ~TableState();

    void reset();
    int getScore();
    
    /* data generale*/
    position_t pos_opponent;
    unsigned long startTime;
    robot_t robot;

    int strategy;

    /* data Show Must Go On*/
    bool avail_stocks[STOCK_COUNT];     // Is stock available
    bool done_banderole;               // Is action done
    int builtTribuneHeights[10];  // Heigh of the tribunes built by us
};

// Serialize tableState
void to_json(json& j, const TableState& ts);