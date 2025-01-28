#include "functions.h"
#include "main.hpp"
#include "navigation.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "constante.h"
#include "arduino.hpp"
#include <math.h>

//TODO : Functions to fill in
int takeStock(int xStart,int yStart, int xEnd, int yEnd, int num_zone){
    return 0;
}
int construct(int x,int y,int theta){
    return 0;
}

// function to construct a single tribune by placing a single platform and pushing the tribune
bool constructSingleTribune(){
    static int state = 1;
    bool res1, res2;
    switch (state)
    {
    case 1:
        if (movePlatformLifts(true))
            state ++;
        break;
    case 2:
        if (movePlatformElevator(1))
            state ++;
        break;
    case 3:
        res1 = movePlatformLifts(false);
        res2 = moveTribunePusher(true);
        if (res1 && res2){
            state++;
        }
        break;
    case 4:
        res1 = movePlatformLifts(true);
        res2 = moveTribunePusher(false);
        if (res1 && res2){
            state = 1;
            return true;
        }
        break;
    }
    return false;
}

// function to take platforms from a stock
bool takeStockPlatforms(){
    static int state = 0;
    switch (state)
    {
    case 0:
        if (movePlatformLifts(true)) // Move the platforms inside
            state ++;
        break;
    case 1:
        if (movePlatformElevator(0)) // Move the elevator down
            state ++;
        break;
    case 2:
        if (movePlatformLifts(false)) // Move the platforms outside
            state ++;
        break;
    case 3:
        if (movePlatformElevator(2)){ // Move the elevator up
            state = 0;
            return true;
        }
        break;
    }
    return false;
}

// This shit is clean af
bool movePlatformLifts(bool inside){
    static unsigned long startTime = _millis();
    static bool previousInside = !inside;
    if (previousInside != inside){
        startTime = _millis(); // Reset the timer
        previousInside = inside;
        arduino.moveServo(PLATFORMS_LIFT_LEFT_SERVO_NUM, inside ? 90 : 0); // TODO : Check if this is correct
        arduino.moveServo(PLATFORMS_LIFT_RIGHT_SERVO_NUM, inside ? 0 : 90);
    }
    return (_millis() > startTime + 1000); // delay
}

// Moves the platforms elevator to a predefined level
// 0:lowest, 1:middle, 2:highest
bool movePlatformElevator(int level){
    static int previousLevel = -1;

    int target = 0;
    switch (level)
    {
    case 0:
        target = 0; break;
    case 1:
        target = 1000; break;
    case 2:
        target = 12000; break;
    }
    if (previousLevel != level){
        previousLevel = level;
        arduino.moveStepper(target, PLATFORMS_ELEVATOR_STEPPER_NUM); // TODO : Check if this is correct
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, PLATFORMS_ELEVATOR_STEPPER_NUM)) return true;
    return (currentValue == target);
}

// Moves the tribune elevator to a predefined level
bool moveTribuneElevator(bool high){
    static bool previousHigh = !high;

    int target = high ? 2000 : 0;
    if (previousHigh != high){
        previousHigh = high;
        arduino.moveStepper(target, TRIBUNES_ELEVATOR_STEPPER_NUM); // TODO : Check if this is correct
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, TRIBUNES_ELEVATOR_STEPPER_NUM)) return true;
    return (currentValue == target);
}

bool moveTribunePusher(bool outside){
    static unsigned long startTime = _millis();
    static bool previousOutside = !outside;
    if (previousOutside != outside){
        startTime = _millis(); // Reset the timer
        previousOutside = outside;
        arduino.moveServo(TRIBUNES_PUSH_SERVO_NUM, outside ? 120 : 20); // TODO : Check if this is correct
    }
    return (_millis() > startTime + 2000); // delay
}



void resetActionneur(){
    for (int i = 0; i < 4; i++){
        arduino.enableStepper(i);
    }
    movePlatformLifts(true);
    movePlatformElevator(0);
    moveTribunePusher(false);
    moveTribuneElevator(false);
}
void disableActionneur(){
    arduino.disableStepper(1);
    arduino.disableStepper(2);
    arduino.disableStepper(3);
    arduino.disableStepper(4);
}

// TODO : Remove ? Not even used..
int returnToHome(){
    int home_x = 700;
    int home_y = tableStatus.robot.colorTeam == YELLOW ? 1200 : -1200;
    nav_return_t res = navigationGoToNoTurn(home_x, home_y);
    return res == NAV_DONE;
}


void blinkLed(int LED_ID,int periode){
    static unsigned long startTime = _millis();
    static int step = 0;

    if(step == 0 && startTime < _millis()){
        arduino.ledOn(LED_ID);
        step++;
        startTime = _millis()+periode/2;
    }
    else if(step == 1 && startTime < _millis()){
        arduino.ledOff(LED_ID);
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

void opponentInAction(position_t* position){ //TODO : Check if this is correct
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

colorTeam_t readColorSensorSwitch(){
    bool sensor = 0;
    if (!arduino.readSensor(2, sensor)) return NONE;
    return sensor ? YELLOW : BLUE;
}