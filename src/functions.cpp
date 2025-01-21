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
    // Stop the robot 
    // TODO
    arduino.moveStepper(0, 1);
    arduino.moveStepper(0, 2);
    arduino.moveStepper(0, 3);
    arduino.moveStepper(0, 4);
    arduino.servoPosition(1, 90);
    arduino.servoPosition(2, 90);
    arduino.servoPosition(3, 90);
    arduino.servoPosition(4, 90);
    arduino.servoPosition(5, 90);
    arduino.servoPosition(6, 90);
    arduino.disableStepper(1);
    arduino.disableStepper(2);
    arduino.disableStepper(3);
    arduino.disableStepper(4);
}

void enableActionneur(){
    arduino.enableStepper(1);
    arduino.enableStepper(2);
    arduino.enableStepper(3);
    arduino.enableStepper(4);
    //arduino.setStepper(1, 0);
    //arduino.setStepper(2, 0);
    //arduino.setStepper(3, 0);
    //arduino.setStepper(4, 0);
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

// Function to check if a point (px, py) lies inside the rectangle
bool m_isPointInsideRectangle(float px, float py, float cx, float cy, float w, float h) {
    float left = cx - w / 2, right = cx + w / 2;
    float bottom = cy - h / 2, top = cy + h / 2;
    return (px >= left && px <= right && py >= bottom && py <= top);
}

void opponentInAction(position_t* position){
    const int OPPONENT_ROBOT_RADIUS = 200; //200mm
    for (int i = 0; i < STOCK_COUNT; i++){
        if (tableStatus.stock[i].etat == false)
            continue;
        position_t stock_pos = STOCK_POSITION_ARRAY[i];
        int w = stock_pos.theta == 0 ? 300 : 0;
        int h = stock_pos.theta == 90 ? 300 : 0;
        if (m_isPointInsideRectangle(position->x, position->y, stock_pos.x, stock_pos.y, OPPONENT_ROBOT_RADIUS * 2 + w, OPPONENT_ROBOT_RADIUS * 2 + h)){
            tableStatus.stock[i].etat = false;
            LOG_GREEN_INFO("opponent has taken stock #", i, " / x = ", position->x , " / y = ", position->y);
            break;
        }
    }
}