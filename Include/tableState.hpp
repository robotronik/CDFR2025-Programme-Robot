#pragma once

#include "config.h"
#include "utils.h"
#include "actionContainer.hpp"


class tableState
{
public:
    /* data */
    colorTeam_t panneauSolaireRotate[9];
    
    

    colorTeam_t colorTeam;
    robot_t* nous,*adversaire;

    bool solarPanelTurn = false;
    unsigned long startTime;
    int dimensionZone;
    
    actionContainer* Actions;

    //Poistion
    const cartesian_position_t plantPosition[6] =
    {{500,0},{300,-500},{-300,-500},{-500,0},{-300,500},{300,500}};
    const cartesian_position_t JardinierePosition[6] =
    {{387,-1500,YELLOW},{-387,-1500,BLUE},{-1000,-762,BLUE},{-1000,762,YELLOW},{-387,1500,YELLOW},{387,1500,BLUE}};
    const obstacle_t action_obstacle[NOMBRE_ACTIONS] = {{0,0,0}};
    const cartesian_position_t centre_action[6] = {{100,100},{10,10}}; //coordonnées des centres des actions
    int value_actions[NOMBRE_ACTIONS] = {4,3,2,1};

public:
    tableState(/* args */);
    ~tableState();
};

