#include "actions/functions.h"
#include "main.hpp"
#include "navigation/navigation.h"
#include "navigation/highways.h"
#include "lidar/lidarAnalize.h"
#include "lidar/Lidar.hpp"
#include "defs/constante.h"
#include "i2c/Arduino.hpp"
#include "actions/strats.hpp"
#include <math.h>

// ------------------------------------------------------
//                   BASIC FSM CONTROL
// ------------------------------------------------------

// function to construct a single tribune by placing a single platform and pushing the tribune
bool constructSingleTribune(){
    static int state = 1;
    static unsigned long startTime;
    switch (state)
    {
    case 1:
        if (movePlatformLifts(0))
            state ++;
        break;
    case 2:
        if (movePlatformElevator(1))
            state ++;
        break;
    case 3:
        if (movePlatformLifts(2, true) & moveTribunePusher(true, true)){
            startTime = _millis();
            state++;
        }
        break;
    case 4:
        if (_millis() > startTime + 400){
            state++;
        }
        break;
    case 5:
        if (movePlatformLifts(1, false) & moveTribunePusher(false) & moveClaws(1)){
            state = 1;
            return true;
        }
        break;
    }
    return false;
}

// function to take platforms from a stock
bool takeStockPlatforms(){
    static int state = 1;
    static unsigned long startTime;
    switch (state)
    {
    case 1 :
        // Move the platforms lifts inside and move elevator down
        if (movePlatformLifts(0) & movePlatformElevator(0)){
            // Move the platforms lifts outside and move elevator up
            startTime = _millis();
            state ++;
        }
        break;
    case 2:
        movePlatformLifts(1);
        movePlatformElevator(2);
        if (_millis() > startTime + 600){
            state = 1;
            return true;
        }
        break;
    }
    return false;
}

bool liftSingleTribune(){
    static int state = 1;
    static unsigned long startTime = _millis();
    switch (state)
    {
    case 1:
        if (moveClaws(1)){
            moveTribuneElevator();
            startTime = _millis();
            state ++;
        }
        break;
    case 2:
        if (_millis() > startTime + 2300)
            state ++;
        break;
    case 3:
        if (moveClaws(2))
            state++;
        break;
    case 4:
        if (_millis() > startTime + 5500){
            state = 1;
            return true;
        }
        break;
    }
    return false;
}
bool deployBanner(){
    static int state = 1;
    switch (state)
    {
    case 1:
        if (moveBannerDeploy(true)){
            state++;
        }
        break;
    case 2:
        if (moveBannerDeploy(false)){
            state = 1;
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
// pos = 0: inside, 1: middle, 2: outside
bool movePlatformLifts(int pos, bool slow){
    static int previousPos = !pos;
    int target_left = 0;
    int target_right = 0;
    switch (pos)
    {
    case 0:
        target_left = 140;
        target_right= 0; 
        break;
    case 1:
        target_left = 75;
        target_right= 70; 
        break;
    case 2:
        target_left = 35;
        target_right= 100; 
        break;
    }
    if (previousPos != pos){
        previousPos = pos;
        int speed = slow ? 100 : 400;
        arduino.moveServoSpeed(PLATFORMS_LIFT_LEFT_SERVO_NUM, target_left, speed);
        arduino.moveServoSpeed(PLATFORMS_LIFT_RIGHT_SERVO_NUM,target_right,speed);
    }
    int current_left = 0;
    if (!arduino.getServo(PLATFORMS_LIFT_LEFT_SERVO_NUM, current_left)) return false;
    return current_left == target_left;
}

bool moveTribunePusher(bool outside, bool slow){
    static bool previousOutside = !outside;
    int target = outside ? 0 : 90;
    if (previousOutside != outside){
        previousOutside = outside;
        arduino.moveServoSpeed(TRIBUNES_PUSH_SERVO_NUM, target, slow ? 100 : 400);
    }
    int current = 0;
    if (!arduino.getServo(TRIBUNES_PUSH_SERVO_NUM, current)) return false;
    return current == target;
}

// 0 : Fully open
// 1 : Straight
// 2 : Closed
// 3 : Collecting
bool moveClaws(int level){
    static unsigned long startTime = _millis();
    static int previouslevel = !level;

    int target = 0;
    switch (level)
    {
    case 0:
        target = 150; break;
    case 1:
        target = 90; break;
    case 2:
        target = 0; break;
    case 3:
        target = 60; break;
    }

    if (previouslevel != level){
        startTime = _millis(); // Reset the timer
        previouslevel = level;
        arduino.moveServoSpeed(TRIBUNES_CLAWS_SERVO_NUM, target, 400);
    }
    int current = 0;
    if (!arduino.getServo(TRIBUNES_CLAWS_SERVO_NUM, current)) return false;
    return current == target;
}

bool moveBannerDeploy(bool outside){
    return true; // TODO REMOVE AFTER SERVO NM


    static bool previousOutside = !outside;
    int target = outside ? 180 : 0;
    if (previousOutside != outside){
        previousOutside = outside;
        arduino.moveServo(-1, target); //TODO SERVO NUM
    }
    int current = 0;
    if (!arduino.getServo(-1, current)) return false; //TODO SERVO NUM
    return current == target;
}

// ------------------------------------------------------
//                   STEPPER CONTROL
// ------------------------------------------------------

// Moves the platforms elevator to a predefined level
// -1:startpos, 0:lowest, 1:middle, 2:highest
bool movePlatformElevator(int level){
    static int previousLevel = -100;

    int target = 0;
    switch (level)
    {
    case -1:
        target = 0; break;
    case 0:
        target = 500; break;
    case 1:
        target = 3000; break;
    case 2:
        target = 10000; break;
    }
    if (previousLevel != level){
        previousLevel = level;
        arduino.moveStepper(target, PLATFORMS_ELEVATOR_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, PLATFORMS_ELEVATOR_STEPPER_NUM)) return false; // TODO Might need to change this (throw error)
    return (currentValue == target);
}
bool moveColumnsElevator(bool up){
    static bool previousLevel = !up;

    int target = up ? 13000 : 0;
    if (previousLevel != up){
        previousLevel = up;
        arduino.moveStepper(target, COLOMNS_ELEVATOR_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, COLOMNS_ELEVATOR_STEPPER_NUM)) return false; // TODO Might need to change this (throw error)
    return (currentValue == target);
}

// Moves the tribune elevator to a predefined level
bool moveTribuneElevator(){
    arduino.moveMotorDC(50, 30);
    return true;
}

void stopTribuneElevator(){
    arduino.stopMotorDC();
}

// Move the lower revolver to an absolute position by N relative to the pusher
bool moveLowColumnsRevolverAbs(int N){
    static int prevAbsSteps = 0;

    float gearTheets = 13; // Theets per rotation
    float stepperSteps = 200 * 16; // 16 microstepping
    float intervalBetweenN = 3.5; // Theets between N
    int absSteps = (int)((stepperSteps * intervalBetweenN * N) / gearTheets);

    LOG_DEBUG("Moving low revolver to ", N, ", steps pos:", absSteps);

    if (prevAbsSteps != absSteps){
        prevAbsSteps = absSteps;
        arduino.moveStepper(absSteps, COLOMNS_REVOLVER_LOW_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, COLOMNS_REVOLVER_LOW_STEPPER_NUM)) return false;
    return (currentValue == absSteps);
}

// ------------------------------------------------------
//                GLOBAL SET/RES CONTROL
// ------------------------------------------------------


// Returns true if actuators are home
bool homeActuators(){
    stopTribuneElevator();
    return (
    movePlatformLifts(0) &
    moveTribunePusher(false) &
    moveClaws(1) & //0
    movePlatformElevator(-1) &
    moveColumnsElevator(false)
    );
}
void enableActuators(){
    for (int i = 0; i < 4; i++){
        arduino.enableStepper(i);
    }
    arduino.enableServos();
    asserv.set_motor_state(true);
    asserv.set_brake_state(false); 
}
void disableActuators(){
    stopTribuneElevator();
    for (int i = 0; i < 4; i++){
        arduino.disableStepper(i);
    }
    arduino.disableServos();
    asserv.set_motor_state(false);
    asserv.set_brake_state(true); 
    asserv.stop();
}


// ------------------------------------------------------
//                        OTHER
// ------------------------------------------------------

void setStockAsRemoved(int num){
    tableStatus.avail_stocks[num] = false;
    obs_obj_stocks[num].present = false;
    LOG_INFO("Removed stock ", num);
}

bool returnToHome(){
    unsigned long time = _millis() - tableStatus.startTime;
    int home_x = (time < 95000) ? -300 : -600;
    int home_y = (tableStatus.robot.colorTeam == BLUE) ? 1100 : -1100;
    nav_return_t res = navigationGoToNoTurn(home_x, home_y);
    return res == NAV_DONE && isRobotInArrivalZone(tableStatus.robot.pos);
}

// Function to check if a point (px, py) lies inside the rectangle
bool m_isPointInsideRectangle(float px, float py, float cx, float cy, float w, float h) {
    float left = cx - w / 2, right = cx + w / 2;
    float bottom = cy - h / 2, top = cy + h / 2;
    return (px >= left && px <= right && py >= bottom && py <= top);
}

void opponentInAction(position_t position){ //TODO : Check if this is correct
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
    // Update the opponent robot's position in highways
    obs_obj_opponent.pos.x = position.x;
    obs_obj_opponent.pos.y = position.y;
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
            arduino.RGB_Blinking(0, 0, 255);
            break;
        case YELLOW:
            LOG_INFO("Switching to YELLOW");
            arduino.RGB_Blinking(255, 56, 0);
            break;
        default:
            break;
        }

        position_t pos = StratStartingPos();
        asserv.set_coordinates(pos.x, pos.y, pos.theta);
    }
}
void switchStrategy(int strategy){
    if (currentState == RUN) return;
    if (strategy < 1 || strategy > 4){
        LOG_ERROR("Invalid strategy");
        return;
    }
    if (strategy != tableStatus.strategy){
        LOG_INFO("Strategy switch detected");
        tableStatus.strategy = strategy;
        position_t pos = StratStartingPos();
        asserv.set_coordinates(pos.x, pos.y, pos.theta);
    }
}
void getBestAvailableStock(){
    // Returns all the stocks available and their position
    // TODO list all the available stocks and their positions
    // then choose the best one
    for (int i = 0; i < STOCK_COUNT; i++){
        if (!tableStatus.avail_stocks[i])
            continue;
        if (tableStatus.robot.colorTeam == BLUE && i == PROTECTED_YELLOW_STOCK)
            continue;
        if (tableStatus.robot.colorTeam == YELLOW && i == PROTECTED_BLUE_STOCK)
            continue;

        int res = getBestStockPositionOff(i, tableStatus.robot.pos);
    }
}

// Returns the count of the available positions 
int getBestStockPositionOff(int stockN, position_t fromPos){
    int bestIndex = 0;
    // Calculate the best position (closest to the robot)
    float minDist = 1000000;
    for (int n = 0; n < 4; n++){
        int map = STOCK_OFFSET_MAPPING[stockN][n];
        if (map < 0) continue;
        position_t offset = STOCK_OFFSETS[map];
        position_t finalPos = STOCK_POSITION_ARRAY[stockN];
        finalPos.x += offset.x;
        finalPos.y += offset.y;
        finalPos.theta = offset.theta;
        
        // Calculate the distance between the robot and the stock
        float dist = position_distance(fromPos, finalPos);
        if (dist < minDist){
            minDist = dist;
            bestIndex = n;
        }

    }
    return bestIndex;
}

bool isRobotInArrivalZone(position_t position){
    // Returns true if the robot is in the arrival zone
    int robotSmallRadius = 100;
    int w = 450;
    int h = 600;
    int c_x = -550 - w/2;
    int c_y = tableStatus.robot.colorTeam == BLUE ? (900 + h/2) : (-900 - h/2);
    return m_isPointInsideRectangle(position.x, position.y, c_x, c_y, w + 2*robotSmallRadius, h + 2*robotSmallRadius);
}

// ------------------------------------------------------
//                    INPUT SENSOR
// ------------------------------------------------------

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

bool readLeftPusherSensor(){
    bool state;
    if (!arduino.readSensor(PUSHER_LEFT_SENSOR_NUM, state)) return false;
    return state;
}
bool readRightPusherSensor(){
    bool state;
    if (!arduino.readSensor(PUSHER_RIGHT_SENSOR_NUM, state)) return false;
    return state;
}
// Returns true if 2 cans are detected in front of the pusher
bool readPusherSensors(){
    return readLeftPusherSensor() & readRightPusherSensor();
}