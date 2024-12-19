#include "functions.h"
#include "main.hpp"
#include "navigation.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "constante.h"
#include <math.h>

int initPosition2(int x, int y,int theta){
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
        robotI2C.get_coordinates(xSave,ySave,thetaSave);
        robotI2C.set_linear_max_speed(150);
        //startTime = _millis()+100;
        nextState = SETPOS_FIRSTFORWARD;
        break;


    case SETPOS_FIRSTFORWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTFORWARD");
        
        if(navigationGoToNoTurn(xSave,ySave+yBack) == NAV_DONE){
            nextState = SETPOS_FIRSTBACKWARD;
            robotI2C.set_coordinates(xSave,yStart,thetaStart);
        }
        break;

    case SETPOS_FIRSTBACKWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTBACKWARD");    
        if(navigationGoTo(xSave,y,-180, Direction::BACKWARD) == NAV_DONE){
            nextState = SETPOS_SECONDBACKWARD;
        }
        break;

    case SETPOS_SECONDBACKWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDBACKWARD");
        if(navigationGoToNoTurn(xSave+xSecond,y) == NAV_DONE){
            robotI2C.set_coordinates(xStart, y,-180);
            nextState = SETPOS_SECONDFORWARD;
        }
        break;

    case SETPOS_SECONDFORWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDFORWAsetMaxTorqueRD");
        if(navigationGoTo(x,y,-180,Direction::BACKWARD) == NAV_DONE){
            nextState = SETPOS_INIT;
            robotI2C.set_linear_max_speed(MAX_SPEED);
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

//TODO : Functions to fill in
int takeStock(int xStart,int yStart, int xEnd, int yEnd, int num_zone){
    return 0;
}
int construct(int x,int y,int theta){
    return 0;
}





void resetActionneur(){
    arduino.servoPosition(1,180);
    //TODO
    //arduino.servoPosition(2,CLAMPSLEEP);
    //arduino.moveStepper(ELEVATORJARDINIERE,1);
    
}

// TODO : Remove ? Not even used..
int returnToHome(){
    int home_x = 700;
    int home_y = tableStatus.robot.colorTeam == YELLOW ? 1200 : -1200;
    nav_return_t res = navigationGoToNoTurn(home_x, home_y);
    bool breturn = res == NAV_DONE;
    return breturn; 
}


void blinkLed(int LedNb,int periode){
    static unsigned long startTime = _millis();
    static int step = 0;

    if(step == 0 && startTime < _millis()){
        arduino.ledOn(LedNb);
        step++;
        startTime = _millis()+periode/2;
    }
    else if(step == 1 && startTime < _millis()){
        arduino.ledOff(LedNb);
        step = 0;
        startTime = _millis()+periode/2;
    }
    return;
}


void opponentInAction(position_t* position){
    //TODO : Fill this in again
    /*

    double distance;
    for (int i = 0; i < 6; i++){
        distance = sqrt(pow(plantPosition[i].x - position->x,2) + pow(plantPosition[i].y - position->y,2));
        if (distance < rayon[0]) {
            tableStatus.planteStockFull[i].etat = false;
            LOG_GREEN_INFO("opponent IN ACTION PLANT :", i, " / x = ", position->x , " / y = ", position->y);
        }
    }
    for (int i=3; i<6; i++){
        distance = sqrt(pow(table[i] - position->y,2) + pow(900 - position->x,2));
        if (distance < rayon[1]) {
            LOG_GREEN_INFO("opponent IN ACTION SOLAR PANEL:", i, " / x = ", position->x , " / y = ", position->y);
            tableStatus.panneauSolaireRotate[i].etat = true;
            }
    }
    for (int i = 0; i < 6; i++){
        distance = sqrt(pow(JardinierePosition[i].x - position->x,2) + pow(JardinierePosition[i].y - position->y,2));
        if (distance < rayon[2] && tableStatus.robot.colorTeam != tableStatus.JardiniereFull[i].color){
            tableStatus.JardiniereFull[i].etat == true;
            LOG_GREEN_INFO("opponent IN ACTION JARDINIERE :", i, " / x = ", position->x , " / y = ", position->y);
        }
    }
    */
}