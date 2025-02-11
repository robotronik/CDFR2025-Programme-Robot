#include "defs/tableState.hpp"

//game_element_t : etat, cout, tps, color

TableState::TableState(){
    reset();
}

TableState::~TableState(){}

void TableState::reset(){
    score = 1;
    pos_opponent.x = 0; pos_opponent.y = 0;

    robot.pos = {0, 0, 0};
    robot.colorTeam = NONE;

    /* data show must go on*/
    for(int i = 0; i<6;i++){
        zoneFull[i].state = false;
    }
    for(int i = 0; i<STOCK_COUNT;i++){
        stocks[i].state = true;
        stocks[i].points = 100; //TODO
    }
    banderole.state = false;
    banderole.points = 20;

    robot.columns_count = 0;
    robot.plank_count = 0;
}

int TableState::getScore()
{
    return score;
}

void TableState::setScore(int score)
{
    this->score = score;
}

void TableState::incrementScore(int score)
{
    this->score += score;
    LOG_GREEN_INFO("Score = ", getScore());
}

// Define serialization for game_element_t
void to_json(json& j, const game_element_t& t) {
    j = json{{"state", t.state}, {"points", t.points}};
}

// Serialize tableState
void to_json(json& j, const TableState& ts) {
    j = json{
        {"stocks", ts.stocks},
        {"banderole", ts.banderole},
        {"zoneFull", ts.zoneFull},
        {"pos_opponent", ts.pos_opponent},
        {"startTime", ts.startTime},
        {"robot", ts.robot}
    };
}