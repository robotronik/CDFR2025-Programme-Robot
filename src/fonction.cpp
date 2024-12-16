#include "fonction.h"

int initPosition2(TableState* itable, CmdAsserv* iAsser,int x, int y,int theta){
    LOG_SCOPE("initPositon2");
    int ireturn = 0;
    static bool initStat = true;
    static fsminitPos_t currentState = SETPOS_INIT;
    fsminitPos_t nextState = currentState;
    int16_t xSave,ySave,thetaSave;
    //static unsigned long startTime;
    //static int step = -1;

    int thetaStart = -90;
    int xSecond = 210;
    int xStart = 1000 - ROBOT_Y_OFFSET;
    int yStart = 1500 - ROBOT_Y_OFFSET;
    int yBack = 310;
    if(y<0){
        thetaStart = 90;
    }
     if(y<0){
        yBack = -yBack;
    }
    if(y<0){
        yStart = -yStart;
    }
    if(x<0){
        xSecond = -xSecond;
        xStart = -xStart;
    }

    
    switch (currentState)
    {
    case SETPOS_INIT :
        if(initStat) LOG_STATE("SETPOS_INIT");
        iAsser->get_coordinates(xSave,ySave,thetaSave);
        iAsser->set_max_speed_backward(150);
        iAsser->set_max_speed_forward(150);
        //startTime = _millis()+100;
        nextState = SETPOS_FIRSTFORWARD;
        break;


    case SETPOS_FIRSTFORWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTFORWARD");
        
        if(navigationGoToNoTurn(xSave,ySave+yBack) == NAV_DONE){
            nextState = SETPOS_FIRSTBACKWARD;
            iAsser->set_coordinates(xSave,yStart,thetaStart);
        }
        break;

    case SETPOS_FIRSTBACKWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTBACKWARD");    
        if(navigationGoTo(xSave,y,-180, CmdAsserv::MOVE_BACKWARD) == NAV_DONE){
            nextState = SETPOS_SECONDBACKWARD;
        }
        break;

    case SETPOS_SECONDBACKWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDBACKWARD");
        if(navigationGoToNoTurn(xSave+xSecond,y) == NAV_DONE){
            iAsser->set_coordinates(xStart, y,-180);
            nextState = SETPOS_SECONDFORWARD;
        }
        break;

    case SETPOS_SECONDFORWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDFORWAsetMaxTorqueRD");
        if(navigationGoTo(x,y,-180,CmdAsserv::MOVE_BACKWARD) == NAV_DONE){
            nextState = SETPOS_INIT;
            iAsser->set_max_speed_backward(MAX_SPEED);
            iAsser->set_max_speed_forward(MAX_SPEED);
            ireturn = 1;
        }
        break;
        
    default:
        if(initStat) LOG_STATE("default");
        nextState = SETPOS_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}

/*
TODO : Remove

int turnSolarPannel(TableState* itable, CmdAsserv* iAsser,Arduino* arduino){
    LOG_SCOPE("SolarPanel");
    static fsmSolarPanel_t currentState = VITESSEE_INIT;
    fsmSolarPanel_t nextState = currentState;
    static bool initStat = true;
    int ireturn = 0,navigationreturn, offsetRobot1,offsetRobot2;
    const int axeX = 800;
    static int solarPanelNumber;
    int bord, departy, angle, bordOff;
    if(itable->robot.colorTeam == YELLOW){
        offsetRobot1 = 5;
        offsetRobot2 = 30;
        bord = 1530-131;
        bordOff = 1500 - 131;
        angle = -90;
        departy = 1200;
    }
    else{
        offsetRobot1 = -75;
        offsetRobot2 = -95; //-105 avant
        bord = -(1530-131);
        departy = -1200;
        angle = 90;
        bordOff = -(1500-131);
    }
    
    switch (currentState)
    {
    case VITESSEE_INIT :
        iAsser->set_max_speed_backward(250);
        iAsser->set_max_speed_forward(250);
        iAsser->set_max_speed_horloge(150);
        iAsser->set_max_speed_trigo(150);
        nextState = SOLARPANEL_INIT;
        break;

    case SOLARPANEL_INIT :
        
        if(initStat) LOG_STATE("SOLARPANEL_INIT");
        if(itable->robot.colorTeam == YELLOW){
            solarPanelNumber = 0;
        }
        else{
            solarPanelNumber = 8;
        }
        if (navigationGoToNoTurn(axeX,bord) > 0){
            int16_t x,y,theta;
            iAsser->get_coordinates(x,y,theta);
            if (fabs(theta - angle) < 3 && fabs(y-bord) < 10){iAsser->set_coordinates(x,bordOff ,angle);}
            nextState = SOLARPANEL_FIN_INIT;
            iAsser->set_max_speed_backward(SPEED_PLANTE);
            iAsser->set_max_speed_forward(SPEED_PLANTE);
            
        }
            break;

    case SOLARPANEL_FIN_INIT :
        
        if(initStat) LOG_STATE("SOLARPANEL_FIN_INIT");
        navigationreturn = navigationGoTo(axeX,departy, -90,CmdAsserv::MOVE_BACKWARD,CmdAsserv::CLOCKWISE);
        if (navigationreturn > 0){
            nextState = SOLARPANEL_FORWARD;
        }
        break;

    case SOLARPANEL_FORWARD :
        if(initStat) LOG_STATE("SOLARPANEL_FORWARD");
        navigationreturn = navigationGoToNoTurn(axeX,table[solarPanelNumber]-offsetRobot1);
        if (itable->startTime+85000 < _millis()){
            nextState = SOLARPANEL_END;
                }
        if(navigationreturn>0){
            nextState = SOLARPANEL_PUSHFOR; 
        }
        break;

    case SOLARPANEL_PUSHFOR :
        if(initStat) LOG_STATE("SOLARPANEL_PUSHFOR");
        if(pullpush(arduino)){
            itable->incrementScore(SOLAR_PANNEL);
            if(itable->robot.colorTeam == YELLOW){
                itable->panneauSolaireRotate[solarPanelNumber].color = YELLOW;
                solarPanelNumber++;

                if(solarPanelNumber==6){
                    nextState = SOLARPANEL_END;
                }
                else if(solarPanelNumber<3 || !itable->panneauSolaireRotate[solarPanelNumber].etat){
                    
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
            else{
                itable->panneauSolaireRotate[solarPanelNumber].color = BLUE;
                solarPanelNumber--;
                if(solarPanelNumber==2){
                    nextState = SOLARPANEL_END;
                }
                else if(solarPanelNumber>5  || !itable->panneauSolaireRotate[solarPanelNumber].etat){
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
        }
        break;

    case SOLARPANEL_BACKWARD :
        if(initStat) LOG_STATE("SOLARPANEL_BACKWARD");
        navigationreturn = navigationGoToNoTurn(axeX,table[solarPanelNumber]-offsetRobot2);
        if(navigationreturn>0){
            nextState = SOLARPANEL_PUSHBACK;
        }

        break;

    case SOLARPANEL_PUSHBACK :
        if(initStat) LOG_STATE("SOLARPANEL_PUSHBACK");
        if(pullpush(arduino)){
            if(itable->robot.colorTeam == YELLOW){
                nextState = SOLARPANEL_FORWARD;
            }
            else{
                nextState = SOLARPANEL_FORWARD;
            }
        }
        break;

    case SOLARPANEL_END :
        if(initStat) LOG_STATE("SOLARPANEL_END");
        iAsser->set_max_speed_trigo(500);
        iAsser->set_max_speed_horloge(500);
        arduino->moveStepper(2000,1);
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_ERROR("SOLARPANEL_DEFAULT");
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int takePlant2( CmdAsserv* iAsser,Arduino* arduino,TableState*itable,int xStart,int yStart, int xEnd, int yEnd, int num_zone){
    LOG_SCOPE("take plant");
    int ireturn = 0;
    static bool initStat = true;
    static fsmtakePlant_t currentState = TAKEPLANT_INIT;
    fsmtakePlant_t nextState = currentState;
    int navigationreturn;
    int xtogo = xEnd - (115/sqrt((xEnd-xStart)*(xEnd-xStart)+(yEnd-yStart)*(yEnd-yStart)))*(xEnd-xStart);
    int ytogo = yEnd - (115/sqrt((xEnd-xStart)*(xEnd-xStart)+(yEnd-yStart)*(yEnd-yStart)))*(yEnd-yStart);

    switch (currentState)
    {
    case TAKEPLANT_INIT :
        if(initStat) LOG_STATE("TAKEPLANT_INIT");
        nextState = TAKEPLANT_FORWARD;
        arduino->servoPosition(4,100);
        iAsser->set_max_speed_forward(200);
        iAsser->set_max_speed_backward(200);
        arduino->moveStepper(ELEVATORUP,1);
        if (!itable->planteStockFull[num_zone].etat){
            iAsser->set_max_speed_forward(MAX_SPEED);
            iAsser->set_max_speed_backward(MAX_SPEED);
            iAsser->set_max_speed_trigo(500);
            iAsser->set_max_speed_horloge(500);
            ireturn = -1;
            nextState = TAKEPLANT_INIT;}
        
        
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        navigationreturn = navigationGoToNoTurn(xEnd,yEnd);
        if(navigationreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(navigationreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
            iAsser->set_max_speed_forward(MAX_SPEED);
            iAsser->set_max_speed_backward(MAX_SPEED);
        }
        break;
    case TAKEPLANT_BACKWARD :
        if(initStat){ LOG_STATE("TAKEPLANT_BACKWARD");
        }
        navigationreturn = navigationGoToNoTurn(xtogo,ytogo,CmdAsserv::MOVE_BACKWARD);
        if(navigationreturn>=1){
            nextState = TAKEPLANT_TAKE;
        }
        else if(navigationreturn < 0){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
        }
        break;
    case TAKEPLANT_TAKE :
        if(initStat) LOG_STATE("TAKEPLANT_TAKE");
        navigationreturn = catchPlant(arduino);
        if(navigationreturn){
            nextState = TAKEPLANT_END;
        }
        break;
    case TAKEPLANT_END :
        if(initStat) LOG_STATE("TAKEPLANT_END");
        nextState = TAKEPLANT_INIT;
        iAsser->set_max_speed_forward(SPEED_PLANTE);
        iAsser->set_max_speed_backward(SPEED_PLANTE);
        iAsser->set_max_speed_horloge(100);
        iAsser->set_max_speed_trigo(100);
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = TAKEPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int jardinierePutPlant(TableState* itable, CmdAsserv* iAsser,Arduino* arduino,int x,int y,int theta){
    LOG_SCOPE("putPlant");
    int ireturn = 0;
    static bool initStat = true;
    static fsmTPutPlant_t currentState = PUTPLANT_INIT;
    fsmTPutPlant_t nextState = currentState;
    int navigationreturn;


    switch (currentState)
    {
    //-------------------------------------------------------------------
    case PUTPLANT_INIT :
        if(initStat) LOG_STATE("PUTPLANT_INIT");
        iAsser->set_max_speed_forward(200);
        iAsser->set_max_speed_backward(200);
        nextState = PUTPLANT_GOBORDER;
        break;
    //-------------------------------------------------------------------
    case PUTPLANT_GOBORDER :
        if(initStat) LOG_STATE("PUTPLANT_GOBORDER");
        
        navigationreturn = navigationGoTo(x,y,theta,CmdAsserv::FORWARD,CmdAsserv::SHORTEST);
        
        if(navigationreturn > 0){
            iAsser->stop();
            iAsser->brake(true);
            nextState = PUTPLANT_PLACE;
        }
        else if(navigationreturn < 0){
            nextState = PUTPLANT_INIT;
            ireturn = -1;
        }        
        break;
    //-------------------------------------------------------------------
    case PUTPLANT_PLACE :
        if(initStat) LOG_STATE("PUTPLANT_PLACE");
        if(releasePlant(arduino)){
            iAsser->stop();
            iAsser->brake(false);
            iAsser->enable_motor();
            nextState = PUTPLANT_INIT;
            iAsser->set_max_speed_forward(MAX_SPEED);
            iAsser->set_max_speed_backward(MAX_SPEED);
            iAsser->set_max_speed_horloge(500);
            iAsser->set_max_speed_trigo(500);
            ireturn = 1;
        }
        break;
    //-------------------------------------------------------------------
    default:
        if(initStat) LOG_ERROR("default");
        nextState = PUTPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}



bool allJardiniereFull(TableState* itable){
    if(itable->robot.colorTeam == YELLOW){
        return  itable->JardiniereFull[3].etat && itable->JardiniereFull[4].etat && itable->JardiniereFull[0].etat;
    }
    else{
        return itable->JardiniereFull[1].etat && itable->JardiniereFull[2].etat && itable->JardiniereFull[5].etat;
    }
}
bool DeuxJardiniereFull(TableState* itable){
    if(itable->robot.colorTeam == YELLOW){
        return  itable->JardiniereFull[3].etat && itable->JardiniereFull[4].etat;
    }
    else{
        return itable->JardiniereFull[1].etat && itable->JardiniereFull[2].etat;
    }
}

bool allStockPlanteUsed(TableState* itable){
    for (int i = 0; i < 6; i++){
        if (itable->planteStockFull[i].etat == true) {return false;}
    }
    return true;
}

*/

//TODO : Functions to fill in
int takeStock( CmdAsserv* iAsser,Arduino* arduino,TableState*itable,int xStart,int yStart, int xEnd, int yEnd, int num_zone){
    return 0;
}
int construct(TableState* itable, CmdAsserv* iAsser,Arduino* arduino,int x,int y,int theta){
    return 0;
}





void resetActionneur(CmdAsserv* iAsser, Arduino* arduino){
    arduino->servoPosition(1,180);
    //TODO
    //arduino->servoPosition(2,CLAMPSLEEP);
    //arduino->moveStepper(ELEVATORJARDINIERE,1);
    
}


int returnToHome(TableState* itable,CmdAsserv* iAsser){
    int home_x = 700;
    int home_y = itable->robot.colorTeam == YELLOW ? 1200 : -1200;
    nav_return_t res = navigationGoTo(home_x, home_y);
    bool breturn = res == NAV_DONE;
    return breturn; 
}


void opponentInAction(TableState* itable, position_t* position){
    //TODO : Fill this in again
    /*

    double distance;
    for (int i = 0; i < 6; i++){
        distance = sqrt(pow(plantPosition[i].x - position->x,2) + pow(plantPosition[i].y - position->y,2));
        if (distance < rayon[0]) {
            itable->planteStockFull[i].etat = false;
            LOG_GREEN_INFO("opponent IN ACTION PLANT :", i, " / x = ", position->x , " / y = ", position->y);
        }
    }
    for (int i=3; i<6; i++){
        distance = sqrt(pow(table[i] - position->y,2) + pow(900 - position->x,2));
        if (distance < rayon[1]) {
            LOG_GREEN_INFO("opponent IN ACTION SOLAR PANEL:", i, " / x = ", position->x , " / y = ", position->y);
            itable->panneauSolaireRotate[i].etat = true;
            }
    }
    for (int i = 0; i < 6; i++){
        distance = sqrt(pow(JardinierePosition[i].x - position->x,2) + pow(JardinierePosition[i].y - position->y,2));
        if (distance < rayon[2] && itable->robot.colorTeam != itable->JardiniereFull[i].color){
            itable->JardiniereFull[i].etat == true;
            LOG_GREEN_INFO("opponent IN ACTION JARDINIERE :", i, " / x = ", position->x , " / y = ", position->y);
        }
    }
    */
}

//TODO : Remove
/*
int VolPlante(TableState* itable, CmdAsserv* iAsser,Arduino* arduino,int x,int y,int theta){
    LOG_SCOPE("putPlant");
    int ireturn = 0;
    static bool initStat = true;
    static fsmVolPlant_t currentState = VOLPLANT_INIT;
    fsmVolPlant_t nextState = currentState;
    int navigationreturn;


    switch (currentState)
    {
    //-------------------------------------------------------------------
    case VOLPLANT_INIT :
        if(initStat) LOG_STATE("VOLPLANT_INIT");
        iAsser->set_max_speed_backward(200);
        iAsser->set_max_speed_forward(200);
        arduino->moveStepper(2000,1);
        nextState = VOLPLANT_GOBORDER;
        break;
    //-------------------------------------------------------------------
    case VOLPLANT_GOBORDER :
        if(initStat) LOG_STATE("VOLPLANT_GOBORDER");
        
        navigationreturn = navigationGoTo(x,y,theta,CmdAsserv::FORWARD,CmdAsserv::SHORTEST);
        
        if(navigationreturn > 0){
            iAsser->stop();
            iAsser->brake(true);
            nextState = VOLPLANT_PLACE;
        }
        else if(navigationreturn < 0){
            nextState = VOLPLANT_INIT;
            ireturn = -1;
        }        
        break;
    //-------------------------------------------------------------------
    case VOLPLANT_PLACE :
        if(initStat) LOG_STATE("VOLPLANT_PLACE");
        if(catchPlant2(arduino)){
            iAsser->stop();
            iAsser->brake(false);
            iAsser->enable_motor();
            nextState = VOLPLANT_BACKWARD;
            iAsser->set_max_speed_backward(MAX_SPEED);
            iAsser->set_max_speed_forward(MAX_SPEED);
            
        }
        break;
    //-------------------------------------------------------------------
    case VOLPLANT_BACKWARD :
        if (initStat) LOG_STATE("VOLPLANT_BACKWARD");
        navigationreturn = navigationGoTo(x,y-200,theta,CmdAsserv::MOVE_BACKWARD,CmdAsserv::SHORTEST);
        if(navigationreturn>0){
            arduino->moveStepper(1600,1);
            nextState = VOLPLANT_ZONEFIN;
            
        }
        else if(navigationreturn < 0){
            nextState = VOLPLANT_INIT;
            ireturn = -1;
        }
        break;

    //-------------------------------------------------------------------
    case VOLPLANT_ZONEFIN :
        if (initStat) LOG_STATE("VOLPLANT_ZONEFIN");
        navigationreturn = navigationGoTo(-200,1250,0,CmdAsserv::FORWARD,CmdAsserv::SHORTEST);
        if(navigationreturn>0){
            nextState = VOLPLANT_FIN;
        }
        else if(navigationreturn < 0){
            nextState = VOLPLANT_INIT;
            ireturn = -1;
        }
        break;
    //-------------------------------------------------------------------
    case VOLPLANT_FIN : 
    if (initStat) LOG_STATE("VOLPLANT_FIN");
        if(FastReleasePlant(arduino)){
            iAsser->stop();
            iAsser->brake(false);
            iAsser->enable_motor();
            nextState = VOLPLANT_INIT;
            itable->FIN = true;
            ireturn = 1;
        }
        break;
    //-------------------------------------------------------------------
    default:
        if(initStat) LOG_ERROR("default");
        nextState = VOLPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}
*/