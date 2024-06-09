#include "tableState.hpp"

//table_t : etat, cout, tps, color

tableState::tableState(/* args */){
    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true;
        zoneFull[i].etat = false; 
        JardiniereFull[i].etat = false;
        planteStockFull[i].cout = 50;
        JardiniereFull[i].cout = 75; 
    }
    solarPanelTurn.cout = 90;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i].color = NONE;
        panneauSolaireRotate[i].etat = 0;
    }
    for(int i =0; i < 4; i++){
        jardiniereFree[i].etat = false;  
        jardiniereFree[i].cout = 100;
    }
        
}

tableState::~tableState(){
}
