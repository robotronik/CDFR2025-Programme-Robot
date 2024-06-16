#include "tableState.hpp"

//table_t : etat, cout, tps, color

tableState::tableState(){
    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true; //test panneau solaire = false
        zoneFull[i].etat = false;
        JardiniereFull[i].etat = false;
        planteStockFull[i].cout = 50;
        JardiniereFull[i].cout = 75;
        jardiniereFree[i].etat =false;
        jardiniereFree[i].cout = 50;
    }

    planteStockFull[3].cout -= 50;
    jardiniereFree[2].etat = true;
    jardiniereFree[3].etat = true;
    solarPanelTurn.etat = false; //test wait solar etat = true
    solarPanelTurn.cout = 50;

    score = 1;
    init.x = 0; init.y = 0; init.teta = -90;nb = 0;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i].color = NONE;
        panneauSolaireRotate[i].etat = false; 
    }
    
    dx = 0; dy = 0;
    prev_pos.x = 0; prev_pos.y = 0;prev_pos.teta=0;
    ennemie.x = 0;ennemie.y = 0;
        
}

tableState::~tableState(){
}

int tableState::getScore()
{
    return score;
}

void tableState::setScore(int score)
{
    this->score = score;
}

void tableState::incrementScore(int score)
{
    this->score += score;
    
}
