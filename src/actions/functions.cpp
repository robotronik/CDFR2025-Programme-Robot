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
        if (movePlatformElevator(4))
            state ++;
        break;
    case 2:
        if (movePlatformElevator(1) & movePlatformLifts(0))
            state ++;
            startTime = _millis();
        }
        break;
    case 3:
        if( movePlatformLifts(2, true) & moveTribunePusher(true, true) & movePlatformElevator(3)){
            state++;
            startTime = _millis();
        }
        break;
    case 4:
        if (_millis() > startTime + 300){
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
            state ++;
        }
        break;
    case 2:
        if (movePlatformLifts(3) & movePlatformElevator(1) & moveClaws(0)){
            state++;
        }
        break;
    case 3:
        if (movePlatformLifts(3) & movePlatformElevator(1) & moveClaws(0)){
            state++;
        }
        break;
    case 3:
        movePlatformLifts(1);
        movePlatformElevator(2);
        state = 1;
        return true;
        state = 1;
        return true;
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
        moveClaws(1);
        if (_millis() > startTime + 500){
        moveClaws(1);
        if (_millis() > startTime + 500){
            moveTribuneElevator();
            startTime = _millis();
            state ++;
        }
        break;
    case 2:
        if (_millis() > startTime + 650)
        if (_millis() > startTime + 650)
            state ++;
        break;
    case 3:
        if (moveClaws(2))
            state++;
        break;
    case 4:
        if (_millis() > startTime + 2000){
        if (_millis() > startTime + 2000){
            state = 1;
            return true;
        }
        break;
    }
    return false;
}
bool deployBannerFront(){
    return deployBanner(true);
}
bool deployBannerBack(){
    return deployBanner(false);
}
bool deployBanner(bool front){
    static int state = 0;
    switch (state)
    {
    case 0:
        if (moveColumnsElevator(2)){
            state++;
        }
        break;
    case 1:
        if (moveBannerDeploy(1, front)){
            state++;
            moveColumnsElevator(1);
        }
        break;
    case 2:
        if (moveBannerDeploy(2, front)){
            state++;
        }
        break;
    case 3:
        if (moveBannerDeploy(0, front)){
            state = 0;
            return true;
        }
        break;
    }
    return false;
}

bool liftAllColumns(){
    static int state = 1;
    switch (state)
    {
    case 1:
        if (moveColumnsElevator(0) & moveStringClaws(true))
            state++;
        break;
    case 2:
        if (moveStringClaws(false))
            state++;
        break;
    case 3:
        if (moveColumnsElevator(3)){
            state = 1;
            return true;
        }
        break;
    }
    return false;
}
bool releaseAllColumns(){
    static int state = 1;
    switch (state)
    {
    case 1:
        if (moveColumnsElevator(0))
            state++;
        break;
    case 2:
        if (moveStringClaws(true))
            state++;
        break;
    case 3:
        if (moveColumnsElevator(1))
            state++;
        break;
    case 4:
        if (moveStringClaws(false)){
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
    case 0: //rentrée
        target_left = 140;
        target_right= 0; 
        break;
    case 1: //milieu
        target_left = 60;
        target_right= 75; 
        break;
    case 2: // totalement sortie
        target_left = 35;
        target_right= 100; 
        break;
    case 3: //un petit peu sortie
        target_left = 110;
        target_right= 20; 
        break;
    case 4: //un poil plus sortie
    target_left = 105;
    target_right= 25; 
    }
    if (previousPos != pos){
        previousPos = pos;
        int speed = slow ? 75 : 400;
        int speed = slow ? 75 : 400;
        arduino.moveServoSpeed(PLATFORMS_LIFT_LEFT_SERVO_NUM, target_left, speed);
        arduino.moveServoSpeed(PLATFORMS_LIFT_RIGHT_SERVO_NUM,target_right,speed);
    }
    int current_left = 0;
    if (!arduino.getServo(PLATFORMS_LIFT_LEFT_SERVO_NUM, current_left)) return false;
    return current_left == target_left;
}

bool moveTribunePusher(bool outside, bool slow){
    static bool previousOutside = !outside;
    int target = outside ? 0 : 110;
    int target = outside ? 0 : 110;
    if (previousOutside != outside){
        previousOutside = outside;
        arduino.moveServoSpeed(TRIBUNES_PUSH_SERVO_NUM, target, slow ? 75 : 400);
        arduino.moveServoSpeed(TRIBUNES_PUSH_SERVO_NUM, target, slow ? 75 : 400);
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
        target = 100; break;
        target = 100; break;
    case 1:
        target = 90; break;
    case 2:
        target = 5; break;
        target = 5; break;
    case 3:
        target = 70; break;
        target = 70; break;
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

// 0 : Fully inside
// 1 : Weight deployed
// 2 : Fully outside
bool moveBannerDeploy(int position, bool front){
    static int previousPositionFront = !position;
    static int previousPositionBack = !position;
    int target;
    switch (position)
    {
        case 0:
            target = 120; break;
        case 1:
            target = 160; break;
        case 2:
            target = 180; break;
    }
    if ( (front && previousPositionFront != position) || (!front && previousPositionBack != position)){
        if (front) previousPositionFront = position;
        else previousPositionBack = position;
        arduino.moveServoSpeed(front ? BANNER_RELEASE_FRONT_SERVO_NUM : BANNER_RELEASE_BACK_SERVO_NUM, target, position == 1 ? 30 : 300);
    }
    int current = 0;
    if (!arduino.getServo(front ? BANNER_RELEASE_FRONT_SERVO_NUM : BANNER_RELEASE_BACK_SERVO_NUM, current)) return false;
    return current == target;
}
bool moveStringClaws(bool open){
    static bool previousOpen = !open;
    int target = open ? 180 : 0;
    if (previousOpen != open){
        previousOpen = open;
        arduino.moveServoSpeed(STRING_CLAWS_SERVO_NUM, target, 200);
    }
    int current = 0;
    if (!arduino.getServo(STRING_CLAWS_SERVO_NUM, current)) return false;
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
        target = 700; break; //sous la 1ère planche
    case 1:
        target = 4000; break; //milieux 1ère planche
    case 2:
        target = 11500; break; //haut sous blocage
    case 3: 
        target = 7000; break; // au dessus 1ère planche
    case 4:
        target = 6000; break; // blocage 1ère planche
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
        arduino.moveStepper(target, COLUMNS_ELEVATOR_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, COLUMNS_ELEVATOR_STEPPER_NUM)) return false; // TODO Might need to change this (throw error)
    return (currentValue == target);
}

// Moves the tribune elevator to a predefined level
bool moveTribuneElevator(){
    arduino.moveMotorDC(100, 30);
    arduino.moveMotorDC(100, 30);
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
        arduino.moveStepper(absSteps, COLUMNS_REVOLVER_LOW_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, COLUMNS_REVOLVER_LOW_STEPPER_NUM)) return false;
    return (currentValue == absSteps);
}

// Moves the platforms elevator to a predefined level
// 1:startpos, 0:lowest, 2:Banner release 3:highest
bool moveColumnsElevator(int level){
    static int previousLevel = -100;

    int target = 0;
    switch (level)
    {
    case 0:
        target = 0; break;
    case 1:
        target = 4000; break;
    case 2:
        target = 8000; break;
    case 3:
        target = 20000; break;
    }
    if (previousLevel != level){
        previousLevel = level;
        arduino.moveStepper(target, COLUMNS_ELEVATOR_STEPPER_NUM);
    }
    int32_t currentValue;
    if (!arduino.getStepper(currentValue, COLUMNS_ELEVATOR_STEPPER_NUM)) return false; // TODO Might need to change this (throw error)
    return (currentValue == target);
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
    moveColumnsElevator(false) &
    moveBannerDeploy(0, true) &
    moveBannerDeploy(0, false) &
    moveColumnsElevator(1) &
    moveStringClaws(false)
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