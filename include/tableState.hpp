#pragma once
#include "config.h"

typedef struct 
{
    bool etat; // si l'action a été faite ou non
    int cout; // points que rapporte l'action
    float tps; //temps passé par l'ennemie dans l'action
    colorTeam_t color; //NONE, BLUE or YELLOW
}table_t;

class tableState
{
public:
    /* data */
    table_t planteStockFull[6]; //zonne des plantes
    table_t JardiniereFull[6]; //zone des jardinières
    table_t zoneFull[6];          // zone départ/arrivés
    table_t jardiniereFree[4];    //zonne devant les jardinières
    colorTeam_t panneauSolaireRotate[9]; 
    
    bool solarPanelTurn = false;
    unsigned long startTime;

    robot_t robot;


    tableState(/* args */);
    ~tableState();
};

