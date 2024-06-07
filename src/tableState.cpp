#include "tableState.hpp"

//table_t : etat, cout, tps, color

tableState::tableState(/* args */){
    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true;
        zoneFull[i].etat = false;
        JardiniereFull[i].etat = false;
        planteStockFull[i].cout = 50;
        JardiniereFull[i].cout = 75;
        jardiniereFree[i].etat =false;
        jardiniereFree[i].cout = 50;
    }
    planteStockFull[3].cout -= 10;
    jardiniereFree[2].etat = true;
    jardiniereFree[3].etat = true;
    solarPanelTurn.etat = false;
    solarPanelTurn.cout = 50;
    fin = false;
    dx = 0; dy = 0; nb = 0;
    pos_balise.x = 0; pos_balise.y = 0;pos_balise.teta=0;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i].color = NONE;
        panneauSolaireRotate[i].etat = false; 
    }
    for(int i =0; i < 4; i++){
        
    }
        
}

tableState::~tableState(){
}
