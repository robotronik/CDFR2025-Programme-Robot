#include "tableState.hpp"

//table_t : etat, cout, tps, color

TableState::TableState(){}
void TableState::init(Affichage* i_affichage)
{
    affichage = i_affichage;

    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true; //test panneau solaire = false
        zoneFull[i].etat = false;
        JardiniereFull[i].etat = false;
        planteStockFull[i].cout = 50; //500 si test tous stocks
        JardiniereFull[i].cout = 75;
        jardiniereFree[i].etat =false;
        jardiniereFree[i].cout = 55;
    }

    planteStockFull[2].cout += 2;
    planteStockFull[4].cout += 2;
    planteStockFull[3].cout -= 4;
    jardiniereFree[2].etat = true;
    jardiniereFree[3].etat = true;
    solarPanelTurn.etat = false; //test wait solar etat = true
    solarPanelTurn.cout = 50;

    score = 1;
    FIN = false;
    init_pos.x = 0; init_pos.y = 0; init_pos.teta = -90;nb = 0;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i].color = NONE;
        panneauSolaireRotate[i].etat = false; 
    }
    
    dx = 0; dy = 0;
    prev_pos.x = 0; prev_pos.y = 0;prev_pos.teta=0;
    ennemie.x = 0;ennemie.y = 0; nb =0;
        
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
        {"planteStockFull", ts.planteStockFull},
        {"JardiniereFull", ts.JardiniereFull},
        {"zoneFull", ts.zoneFull},
        {"jardiniereFree", ts.jardiniereFree},
        {"panneauSolaireRotate", ts.panneauSolaireRotate},
        {"dx", ts.dx},
        {"dy", ts.dy},
        {"ennemie", ts.ennemie},
        {"prev_pos", ts.prev_pos},
        {"init_pos", ts.init_pos},
        {"nb", ts.nb},
        {"solarPanelTurn", ts.solarPanelTurn},
        {"startTime", ts.startTime},
        {"FIN", ts.FIN},
        {"robot", ts.robot}
    };
}