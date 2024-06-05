#include "tableState.hpp"

//table_t : etat, cout, tps, color

tableState::tableState(/* args */){
    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true;
        planteStockFull[i].cout = 20;//pas utilisé
        JardiniereFull[i].etat = false;
        JardiniereFull[i].cout = 40; //pas utilisé
        zoneFull[i].etat = false; //pas utilisé
    }
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i] = NONE;        
    }
    for(int i =0; i < 4; i++){
        jardiniereFree[i].etat = false;  
        jardiniereFree[i].cout = 50; //pas utilisé
    }
        
}

tableState::~tableState(){
}
