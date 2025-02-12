#include "defs/tableState.hpp"
#include "utils/logger.hpp"

TableState::TableState(){
    reset();
}

TableState::~TableState(){}

void TableState::reset(){
    pos_opponent.x = 0; pos_opponent.y = 0;

    robot.pos = {0, 0, 0};
    robot.colorTeam = NONE;

    /* data show must go on*/
    for(int i = 0; i<STOCK_COUNT;i++){
        avail_stocks[i] = true;
    }
    done_banderole = false;

    robot.columns_count = 0;
    robot.plank_count = 0;

    for (int i = 0; i < 10; i++){
        builtTribuneHeights[i] = 0;
    }
}

int TableState::getScore()
{
    int totalScore = 0; //TODO Maybe start at 1 ?
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
        {"robot", ts.robot}
    };
}