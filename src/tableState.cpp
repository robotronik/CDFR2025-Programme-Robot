#include "tableState.hpp"

//table_t : etat, cout, tps, color

tableState::tableState(/* args */){
    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true;
        zoneFull[i].etat = false; //pas utilisé
        JardiniereFull[i].etat = false;
        planteStockFull[i].cout = 50;//pas utilisé
        JardiniereFull[i].cout = 75; //pas utilisé
        
    }
    solarPanelTurn.cout = 90;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i] = NONE;        
    }
    for(int i =0; i < 4; i++){
        jardiniereFree[i].etat = false;  
        jardiniereFree[i].cout = 100; //pas utilisé
    }
        
}

tableState::~tableState(){
}
