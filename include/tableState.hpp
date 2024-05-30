#pragma once


#include "utils.h"


class tableState
{
public:
    /* data */
    bool planteStockFull[6];
    bool JardiniereFull[6];
    bool zoneFull[6];
    bool jardiniereFree[4];
    colorTeam_t panneauSolaireRotate[9];
    bool robotHavePlante;
    colorTeam_t colorTeam;
    bool solarPanelTurn = false;
    unsigned long startTime;

    //Poistion
    const cartesian_position_t plantPosition[6] =
    {{500,0},{300,-500},{-300,-500},{-500,0},{-300,500},{300,500}};
    const cartesian_position_t JardinierePosition[6] =
    {{387,-1500,YELLOW},{-387,-1500,BLUE},{-1000,-762,BLUE},{-1000,762,YELLOW},{-387,1500,YELLOW},{387,1500,BLUE}};

public:
    tableState(/* args */);
    ~tableState();
};

