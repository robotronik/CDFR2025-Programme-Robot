#include "defs/tableState.hpp"

//table_t : etat, cout, tps, color

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
        zoneFull[i].etat = false;
    }
    for(int i = 0; i<10;i++){
        stock[i].etat = true;
        stock[i].cout = 100; //TODO
        stock[i].color = NONE;
    }
    banderole.etat = false;

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
    LOG_GREEN_INFO("score = ", getScore());
}

// Define serialization for table_t
void to_json(json& j, const table_t& t) {
    j = json{{"etat", t.etat}, {"cout", t.cout}, {"tps", t.tps}, {"color", t.color}};
}

// Serialize tableState
void to_json(json& j, const TableState& ts) {
    j = json{
        {"stock", ts.stock},
        {"banderole", ts.banderole},
        {"zoneFull", ts.zoneFull},
        {"pos_opponent", ts.pos_opponent},
        {"startTime", ts.startTime},
        {"robot", ts.robot}
    };
}