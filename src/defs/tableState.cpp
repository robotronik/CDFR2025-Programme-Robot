#include "defs/tableState.hpp"
#include "actions/functions.h"
#include "utils/logger.hpp"

TableState::TableState(){
    pos_opponent.x = 0; pos_opponent.y = 0;
    robot.pos = {0, 0, 0};
    robot.colorTeam = NONE;
    strategy = 1;

    reset();
}

TableState::~TableState(){}

void TableState::reset(){
    
    /* data show must go on*/
    for(int i = 0; i<STOCK_COUNT;i++){
        avail_stocks[i] = true;
    }
    done_banderole = false;
    robot.plank_count = 0;

    for (int i = 0; i < 10; i++){
        builtTribuneHeights[i] = 0;
    }
}

int TableState::getScore()
{
    int totalScore = 1; //TODO
    for (int i = 0; i < 10; i++){
        switch (builtTribuneHeights[i])
        {
        case 0:
            break;
        case 1:
            totalScore += 4;
            break;
        case 2:
            totalScore += 4 + 8;
            break;
        case 3:
            totalScore += 4 + 8 + 16;
            break;
        default:
            LOG_ERROR("Built tribune heigh is at impossible value : ", builtTribuneHeights[i]);
            break;
        }
    }
    if (isRobotInArrivalZone(robot.pos))
        totalScore += 10;

    if (done_banderole)
        totalScore += 20;
    return totalScore;
}

// Serialize tableState
void to_json(json& j, const TableState& ts) {
    j = json{
        {"avail_stocks", ts.avail_stocks},
        {"done_banderole", ts.done_banderole},
        {"pos_opponent", ts.pos_opponent},
        {"startTime", ts.startTime},
        {"robot", ts.robot},
        {"strategy", ts.strategy}
    };
}