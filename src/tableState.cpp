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
    solarPanelTurn.cout = 0;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i] = NONE;        
    }
    for(int i =0; i < 4; i++){
        
    }
        
}

tableState::~tableState(){
}
