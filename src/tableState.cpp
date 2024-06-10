#include "tableState.hpp"

//table_t : etat, cout, tps, color

tableState::tableState(/* args */){
    points = 1;
    for(int i = 0; i<6;i++){
        planteStockFull[i].etat = true;
        zoneFull[i].etat = false; 
        JardiniereFull[i].etat = false;
        planteStockFull[i].cout = 50;
        JardiniereFull[i].cout = 100; 
    }
    planteStockFull[3].cout = 40; //plante proche pamis, moins bien rangé et moins fait donc on garde
    solarPanelTurn.cout = 90;
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i].color = NONE;
        panneauSolaireRotate[i].etat = 0;
    }
    for(int i =0; i < 4; i++){
        jardiniereFree[i].etat = false;  
        jardiniereFree[i].cout = 50;
    }
        
}

tableState::~tableState(){
}
