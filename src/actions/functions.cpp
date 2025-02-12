#include "actions/functions.h"
#include "main.hpp"
#include "navigation/navigation.h"
#include "navigation/highways.h"
#include "lidar/lidarAnalize.h"
#include "lidar/Lidar.hpp"
#include "defs/constante.h"
#include "i2c/Arduino.hpp"
#include <math.h>

//TODO : Functions to fill in
int takeStock(int xStart,int yStart, int xEnd, int yEnd, int num_zone){
    
    setStockAsRemoved(-1); //TODO once has taken stock
    return 0;
}
int construct(int x,int y,int theta){
    return 0;
}

// ------------------------------------------------------
//                   BASIC FSM CONTROL
// ------------------------------------------------------

// function to construct a single tribune by placing a single platform and pushing the tribune
bool constructSingleTribune(){
    static int state = 1;
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
        if (movePlatformLifts(false) & moveTribunePusher(true)){
            state++;
        }
        break;
    case 4:
        if (movePlatformLifts(true) & moveTribunePusher(false)){
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
        if (movePlatformLifts(true)) // Move the platforms lifts inside
            state ++;
        break;
    case 1:
        if (movePlatformElevator(0)) // Move the elevator down
            state ++;
        break;
    case 2:
        if (movePlatformLifts(false)) // Move the platforms lifts outside
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

// ------------------------------------------------------
//                   SERVO CONTROL
// ------------------------------------------------------

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

bool moveTribunePusher(bool outside){
    static unsigned long startTime = _millis();
    static bool previousOutside = !outside;
    if (previousOutside != outside){
        startTime = _millis(); // Reset the timer
        previousOutside = outside;
        arduino.moveServo(TRIBUNES_PUSH_SERVO_NUM, outside ? 180 : 0); // TODO : Check if this is correct
    }
    return (_millis() > startTime + 2000); // delay
}

// ------------------------------------------------------
//                   STEPPER CONTROL
// ------------------------------------------------------

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
        target = 4000; break;
    }
    if (previousLevel != level){
        previousLevel = level;
        arduino.moveStepper(target, PLATFORMS_ELEVATOR_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, PLATFORMS_ELEVATOR_STEPPER_NUM)) return false; // TODO Might need to change this (throw error)
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
    if (!arduino.getStepper(currentValue, TRIBUNES_ELEVATOR_STEPPER_NUM)) return false;
    return (currentValue == target);
}


// ------------------------------------------------------
//                GLOBAL SET/RES CONTROL
// ------------------------------------------------------


// Returns true if actuators are home
bool homeActuators(){
    return 
    movePlatformLifts(true) &
    movePlatformElevator(0) &
    moveTribunePusher(false) &
    moveTribuneElevator(false);
}
void enableActuators(){
    for (int i = 0; i < 4; i++){
        arduino.enableStepper(i);
    }
}
void disableActuators(){
    for (int i = 0; i < 4; i++){
        arduino.disableStepper(i);
    }
}


// ------------------------------------------------------
//                        OTHER
// ------------------------------------------------------

void setStockAsRemoved(int num){
    tableStatus.avail_stocks[num] = false;
    obs_obj_stocks[num].present = false;
    LOG_INFO("Removed stock ", num);
}

// TODO : Remove ? Not even used..
int returnToHome(){
    int home_x = 700;
    int home_y = tableStatus.robot.colorTeam == YELLOW ? 1200 : -1200;
    nav_return_t res = navigationGoToNoTurn(home_x, home_y);
    return res == NAV_DONE;
}

// Function to check if a point (px, py) lies inside the rectangle
bool m_isPointInsideRectangle(float px, float py, float cx, float cy, float w, float h) {
    float left = cx - w / 2, right = cx + w / 2;
    float bottom = cy - h / 2, top = cy + h / 2;
    return (px >= left && px <= right && py >= bottom && py <= top);
}

void opponentInAction(position_t position){ //TODO : Check if this is correct
    const int OPPONENT_ROBOT_RADIUS = 200; //200mm
    for (int i = 0; i < STOCK_COUNT; i++){
        if (tableStatus.avail_stocks[i] == false)
            continue;
        position_t stock_pos = STOCK_POSITION_ARRAY[i];
        int w = stock_pos.theta == 0 ? 300 : 0;
        int h = stock_pos.theta == 90 ? 300 : 0;
        if (m_isPointInsideRectangle(position.x, position.y, stock_pos.x, stock_pos.y, OPPONENT_ROBOT_RADIUS * 2 + w, OPPONENT_ROBOT_RADIUS * 2 + h)){
            setStockAsRemoved(i);
            LOG_GREEN_INFO("opponent has taken stock #", i, " / x = ", position.x , " / y = ", position.y);
            break;
        }
    }
}
void switchTeamSide(colorTeam_t color){
    if (color == NONE) return;
    if (currentState == RUN) return;
    if (color != tableStatus.robot.colorTeam){
        LOG_INFO("Color switch detected");
        tableStatus.robot.colorTeam = color;

        switch (color)
        {
        case BLUE:
            LOG_INFO("Switching to BLUE");
            asserv.set_coordinates(-770, -1390, 90);
            arduino.RGB_Blinking(0, 0, 255);
            break;
        case YELLOW:
            LOG_INFO("Switching to YELLOW");
            asserv.set_coordinates(-770, 1390, -90);
            arduino.RGB_Blinking(255, 255, 0);
            break;
        default:
            break;
        }
    }
}

void getAvailableStockPositions(){
    // Returns all the stocks available and their position
    for (int i = 0; i < STOCK_COUNT; i++){
        if (!tableStatus.stock[i].etat)
            continue;
        if (tableStatus.robot.colorTeam == BLUE && i == PROTECTED_YELLOW_STOCK)
            continue;
        if (tableStatus.robot.colorTeam == YELLOW && i == PROTECTED_BLUE_STOCK)
            continue;

        position_t availPos[4];
        int count = getStockPositions(i, availPos);
        LOG_DEBUG("Stock ", i, " available positions: ", count);
    }
}

// Returns the count of the available positions 
int getStockPositions(int stockN, position_t availPos[4]){
    int availCount = 0;

    for (int n = 0; n < 4; n++){
        int map = STOCK_OFFSET_MAPPING[stockN][n];
        if (map < 0) continue;
        position_t offset = STOCK_OFFSETS[map];
        position_t finalPos = STOCK_POSITION_ARRAY[stockN];
        finalPos.x += offset.x;
        finalPos.y += offset.y;
        finalPos.theta = offset.theta;

        availPos[availCount] = finalPos;
        availCount ++;
    }
    return availCount;
}

bool isRobotInArrivalZone(position_t position){
    // Returns true if the robot is in the arrival zone
    int robotSmallRadius = 100;
    int w = 450;
    int h = 600;
    int c_x = -550 - w/2;
    int c_y = TableState.robot.colorTeam == BLUE ? (900 + h/2) : (-900 - h/2);
    return m_isPointInsideRectangle(position.x, position.y, c_x, c_y, w + 2*robotSmallRadius, h + 2*robotSmallRadius);
}

// ------------------------------------------------------
//                    INPUT SENSOR
// ------------------------------------------------------

colorTeam_t readColorSensorSwitch(){
    bool sensor = 0;
    if (!arduino.readSensor(COLOR_SENSOR_NUM, sensor)) return NONE;
    return sensor ? YELLOW : BLUE;
}

// Returns true if button sensor was high for the last N calls
bool readButtonSensor(){
    static int count = 0;
    bool state;
    if (!arduino.readSensor(BUTTON_SENSOR_NUM, state)) return false;
    if (state)
        count++;
    else
        count = 0;
    return (count >= 5);
}
// Returns true if the latch sensor is disconnected
bool readLatchSensor(){
    static int count = 0;
    static bool prev_state = false;
    bool state;
    if (!arduino.readSensor(LATCH_SENSOR_NUM, state)) return prev_state;
    if (!state)
        count++;
    else
        count = 0;    
    prev_state = state;
    return (count >= 5);
}

// Returns true if both sensors are high
bool readFrontColumnsSensors(){
    bool state1, state2;
    if (!arduino.readSensor(FRONT_COLUMN_SENSOR1_NUM, state1)) return false;
    if (!arduino.readSensor(FRONT_COLUMN_SENSOR2_NUM, state2)) return false;
    return (state1 && state2);
}