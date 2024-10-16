#include "tableState.hpp"

//table_t : etat, cout, tps, color

TableState::TableState(){}
void TableState::init(Affichage* i_affichage)
{
    affichage = i_affichage;
    score = 1;
    FIN = false;
    init_pos.x = 0; init_pos.y = 0; init_pos.teta = -90; nb = 0;
    
    dx = 0; dy = 0;
    prev_pos.x = 0; prev_pos.y = 0;prev_pos.teta=0;
    ennemie.x = 0; ennemie.y = 0; nb = 0;

    robot.pos = {0, 0, 0, 0, 0};
    robot.colorTeam = 0;




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

TableState::~TableState(){
}

int TableState::getScore()
{
    return score;
}

void TableState::setScore(int score)
{
    this->score = score;
    affichage->updateScore(this->score);
}

void TableState::incrementScore(int score)
{
    this->score += score;
    affichage->updateScore(this->score);
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
        {"dx", ts.dx},
        {"dy", ts.dy},
        {"ennemie", ts.ennemie},
        {"prev_pos", ts.prev_pos},
        {"init_pos", ts.init_pos},
        {"nb", ts.nb},
        {"startTime", ts.startTime},
        {"FIN", ts.FIN},
        {"robot", ts.robot}
    };
}