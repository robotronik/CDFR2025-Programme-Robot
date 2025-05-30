#include "actions/revolver.hpp"
#include "utils/logger.hpp"
#include "actions/functions.h"
#include <exception>
#define REVOLVER_SIZE 14

bool emulateActuators = false;

bool lowArr[REVOLVER_SIZE] = {0};   // false = Empty,  true = Occupied
bool highArr[REVOLVER_SIZE] = {0};   // false = Empty,  true = Occupied
int lowBarrelCount = 0;
int lowBarrelShift = 0;
int highBarrelCount = 0;

// Internal prototypes
void DisplayRobot();
void DisplayBarrel();
bool MoveColumns(int direction, int sens);
bool ReleaseLow();
bool isRevolverFull();
bool isRevolverEmpty();
bool LoadHigh();

// Initialize the revolver when the game starts
void initRevolver(){
    for (int i = 0; i < REVOLVER_SIZE; i++){
        lowArr[i] = 0;
        highArr[i] = 0;
    }
    lowBarrelCount = 0;
    lowBarrelShift  = 0;
    highBarrelCount = 0;
}

// -------------------------------------------------
// Display functions
// -------------------------------------------------

// Display the robot structure and the layout of the locations
void DisplayRobot(){
    printf("Affichage Robot \n");
    printf("Etage 1 : \n");
    printf(" 1 2 3 4  \n");
    printf("0       5  \n");
    printf("13  ^   6  \n");
    printf("12      7  \n");
    printf("11 10 9 8  \n\n");
}

// Display the current state of the two barrels in circular and linear form
void DisplayBarrel(){
    printf(" %d %d %d %d    \n",    lowArr[1], lowArr[2], lowArr[3],   lowArr[4]);
    printf("%d       %d     \n",   lowArr[0],                         lowArr[5]);
    printf("%d   ^   %d     \n",   lowArr[13],                        lowArr[6] );
    printf("%d       %d     \n",   lowArr[12],                        lowArr[7]);
    printf(" %d %d %d %d    \n",lowArr[11], lowArr[10], lowArr[9], lowArr[8]);
    printf("\n\n");
}

// -------------------------------------------------
// Functions to handle revolver
// -------------------------------------------------
bool isRevolverFull(){
    if (lowBarrelCount + 4 > REVOLVER_SIZE && (highBarrelCount != 0)) {
        LOG_WARNING("No more space in revolver !");
        return true;
    }
    return false;
}
bool isRevolverEmpty(){
    const int n = REVOLVER_SIZE;
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (lowArr[i] && lowArr[(i + 1) % n]){
            found = true;
            break;
        }
        if (highArr[i] && highArr[(i + 1) % n]){
            found = true;
            break;
        }
    }
    return !found;
}
void ShiftArray(bool arr[], int n, int size) {
    bool* temp = (bool*)malloc(sizeof(bool) * size);
    for (int i = 0; i < size; i++)
        temp[(i + n + size) % size] = arr[i];
    for (int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }
    free(temp);
}

// Spin the barrel by n positions (positive or negative) returns true when done
bool SpinLowBarrel(int n) {
    static int lowBarrelShiftTarget = 0;
    
    if (n == 0)
        return true;

    // Recalcule toujours la cible et initialise
    if (n < REVOLVER_SIZE / 2)
        n += REVOLVER_SIZE;
    if (n > REVOLVER_SIZE / 2)
        n -= REVOLVER_SIZE;

    LOG_INFO("Spin Low Barrel by n=", n, (n > 0) ? " Clockwise" : " Anticlockwise");
    lowBarrelShiftTarget = lowBarrelShift + n;

    if (emulateActuators || moveLowColumnsRevolverAbs(lowBarrelShiftTarget)) {
        ShiftArray(lowArr, n, REVOLVER_SIZE);
        lowBarrelShift = lowBarrelShiftTarget;
        return true;
    }
    return false;
}

//return the shift needed to put first or last 1 to desired position
int ShiftListNumber(bool list[], int desired_position, bool choose_first) {//choose_first = 1 pour mettre le premier 1, 0 pour mettre le dernier 1
    int n = 14, first = -1, last = -1;
    for (int i = 0; i < n; i++) {
        if (list[i] == 1 && list[(i - 1 + n) % n] == 0) 
            first = (first == -1) ? i : first;
        if (list[i] == 1 && list[(i + 1) % n] == 0) 
            last = i;
    }
    if (first == -1) return 0;
    int shift = (desired_position - (choose_first ? first : last)) % n;
    return (shift > n / 2) ? shift - n : shift;
}

int ShiftListPairForRelease(bool list[], int desired_first_pos) {
    const int n = 14;
    for (int i = 0; i < n; ++i) {
        if (list[i] == 1 && list[(i + 1) % n] == 1) {
            // Positionner i et i+1 sur desired_first_pos et desired_first_pos + 1
            int shift = (desired_first_pos - i + n) % n;
            return (shift > n / 2) ? shift - n : shift;
        }
    }
    return 0;
}

    

// Returns true when done
bool MoveColumns(int direction, int sens) { //return 1 when finished sens 1 = monter, 0 = descendre
    LOG_INFO("MoveColumns direction : ", direction, " sens : ", sens);
    int start = (direction == 0 ? 0 : 5),  end = (direction == 0 ? 13 : 6);

    lowArr[start] = lowArr[end] = (sens == 1) ? 0 : 1;;
    //if (!moveServoFloorColumns(sens)) return 0;
    lowBarrelCount -= sens ? 2 : -2;
    DisplayBarrel();
    return true;
}

// -------------------------------------------------
// Functions to handle incomming stock
// -------------------------------------------------

// Load a Stock according to the direction. Barrel needs to be prepared. Called for loading stock
bool RevolverLoadStock(direction_t dir, int num){
    static int prev_num = -1;
    static bool done = false;
    int intake_pos = (dir == FROM_RIGHT) ? 4 : 1;
    int rotation = (dir == FROM_RIGHT) ? -1 : 1; // position ajout column, Sens de rotation
    
    if (isRevolverFull()){
        LOG_ERROR("Revolver is full, cant load stock");
        return true;
    }

    if (prev_num != num) {
        done = false;
        prev_num = num;
    }
    if (!done && SpinLowBarrel(rotation*4)){
        LOG_INFO("Loaded a column from ", (dir == FROM_RIGHT) ? "Right" : "Left");
        for (int i = 0; i < 4; i++) {
            if (lowArr[intake_pos + i * rotation]) 
                throw std::runtime_error("Cant load stock into a already used position!");
            lowArr[intake_pos + i * rotation] = true;
        }
        DisplayBarrel();
        lowBarrelCount += 4;
        done = true;
    }
    return done;
}

// Function that manages the storage of the first level. Returns true when done preparing for intake of stock from direction
bool RevolverPrepareLowBarrel(direction_t dir){
    LOG_INFO("Prerare Low Barrel direction : ",((dir==FROM_RIGHT) ? "right" : "left"));
    if (lowBarrelCount == 0) return true; //no columns in Lowbarrel so position is good
    
    if (lowBarrelCount >= 12) {
        LOG_WARNING("Lower revolver is full, I'll raise the elevator");
        if (highBarrelCount == 0)
            LoadHigh();
    }

    if (!SpinLowBarrel(ShiftListNumber(lowArr, dir ? 4 : 1, dir==FROM_LEFT))) 
        return false;
    return true;
}

// -------------------------------------------------
// Functions to handle outgoing
// -------------------------------------------------

bool RevolverRelease(){
    if (isRevolverEmpty())
        return false;
    // LOG_INFO("RevolverRelease"); //prepare release low barrel
    return ReleaseLow();
}

bool LoadHigh(){
    int state = 1;
    switch (state){
    case 1 :
        //align
        if (true)
            state ++;
        break;
    case 2:
        LOG_INFO("LoadHigh");
        if (liftAllColumns()){
            DisplayBarrel();
            int count = 0;
            for (int i = 4; i < REVOLVER_SIZE; i++){
                count += lowArr[i];
                highArr[i] = lowArr[i];
                lowArr[i] = false;
            }
            lowBarrelCount -= count;
            highBarrelCount += count;
            DisplayBarrel();
            state = 1;
            return true;
        }
        break;
    }
}

bool ReleaseHigh(){
    LOG_INFO("ReleaseHigh");
    if (releaseAllColumns()){
        DisplayBarrel();
        int count = 0;
        for (int i = 4; i < REVOLVER_SIZE; i++){
            count += highArr[i];
            lowArr[i] = highArr[i];
            highArr[i] = false;
        }
        lowBarrelCount += count;
        highBarrelCount -= count;
        DisplayBarrel();
        return true;
    }
}

// Function to release all columns from the barrel
bool ReleaseLow() {
    LOG_INFO("ReleaseLow");
    int shift = ShiftListPairForRelease(lowArr, 2);
    if (shift == 0) {
        LOG_INFO("No Low columns to release");
        return ReleaseHigh();
    }
    if (SpinLowBarrel(shift)){
        DisplayBarrel();
        if (lowBarrelCount == 0) { 
            LOG_INFO("No columns to release");
        } 
        else if (lowArr[2] && lowArr[3]) {
            //readPusherSensors();
            //LOG_WARNING("Ignoring columns sensors");
            if (emulateActuators || readPusherSensors()){
                LOG_INFO("Pusher sensors ok");
                lowArr[2] = 0; // left
                lowArr[3] = 0; // right
                lowBarrelCount -= 2;
                return true;
            }
            else{
                LOG_WARNING("Missing columns detected to be pushed");
                if (!readLeftPusherSensor())
                {
                    LOG_WARNING("Missing columns detected to be pushed on left");
                    lowArr[2] = 0;
                    lowBarrelCount--;
                }
                if (!readRightPusherSensor()){
                    LOG_WARNING("Missing columns detected to be pushed on right");
                    lowArr[3] = 0;
                    lowBarrelCount--;
                }
            }
        }
    }
    return false;
}

// -------------------------------------------------
// Test Functions
// -------------------------------------------------

void TestTakeAction(direction_t dir, int numm){
    if (isRevolverFull())
        return;
    while (!RevolverPrepareLowBarrel(dir));
    while (!RevolverLoadStock(dir, numm));
    DisplayBarrel();
}

void TestRevolver(){
    DisplayRobot();
    DisplayBarrel();

    emulateActuators = true;

    TestTakeAction(FROM_RIGHT, 0);
    TestTakeAction(FROM_LEFT, 1);
    TestTakeAction(FROM_RIGHT, 2);
    TestTakeAction(FROM_LEFT, 3);
    TestTakeAction(FROM_RIGHT, 4);
    TestTakeAction(FROM_LEFT, 5);
    while (!isRevolverEmpty())
        RevolverRelease();

    emulateActuators = false;
    
    LOG_GREEN_INFO("Done Test Revolver !");
    
}