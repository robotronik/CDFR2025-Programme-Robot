#include "actions/revolver.hpp"
#include "utils/logger.hpp"
#include "actions/functions.h"
#include <exception>
#define SIZE_LOW 14

bool emulateActuators = false;

bool lowArr[SIZE_LOW] = {0};   // false = Empty,  true = Occupied
int lowBarrelCount = 0;
int lowBarrelShift = 0;

// Internal prototypes
void DisplayRobot();
void DisplayBarrel();
bool MoveColumns(int direction, int sens);
bool ReleaseLow();
bool isRevolverFull();
bool isRevolverEmpty();

// Initialize the revolver when the game starts
void initRevolver(){
    for (int i = 0; i < SIZE_LOW; i++)
        lowArr[i] = 0;
    lowBarrelCount = 0;
    lowBarrelShift  = 0;
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
    if (lowBarrelCount == SIZE_LOW) {
        LOG_ERROR("No more space in revolver !");
        return true;
    }
    return false;
}
bool isRevolverEmpty(){
    return (lowBarrelCount <= 1);
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
    LOG_INFO("Spin Low Barrel by n=", n, (n > 0) ? " Clockwise" : " Anticlockwise");
    lowBarrelShiftTarget = lowBarrelShift + n;

    if (emulateActuators || moveLowColumnsRevolverAbs(lowBarrelShiftTarget)) {
        ShiftArray(lowArr, n, SIZE_LOW);
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
bool RevolverLoadStock(direction_t dir){
    static int i = 0;
    int intake_pos = (dir == FROM_RIGHT) ? 4 : 1;
    int rotation = (dir == FROM_RIGHT) ? -1 : 1; // position ajout column, Sens de rotation
    
    if (SpinLowBarrel(rotation)){
        LOG_INFO("Loaded a column from ", (dir == FROM_RIGHT) ? "Right" : "Left");
        if (lowArr[intake_pos]) 
            throw std::runtime_error("Cant load stock into a already used position!");
        lowArr[intake_pos] = true;
        DisplayBarrel();
        lowBarrelCount++;
        i++;
        if (i == 4){
            i = 0;
            return true;
        }
    }
    return false;
}

// Function that manages the storage of the first level. Returns true when done preparing for intake of stock from direction
bool RevolverPrepareLowBarrel(direction_t dir){
    LOG_INFO("Prerare Low Barrel direction : ",((dir==FROM_RIGHT) ? "right" : "left"));
    if (lowBarrelCount == 0) return true; //no columns in Lowbarrel so position is good

    if (!SpinLowBarrel(ShiftListNumber(lowArr, dir ? 4 : 1, dir==FROM_LEFT))) 
        return false;

        // TODO Check on that

    int N = (dir == FROM_RIGHT) ? 2 : -2;
    if(lowBarrelCount == 12) {
        if (!SpinLowBarrel(N))
            return false;
        MoveColumns(dir, 1);      //si on fait preparelowbarrel en boucle, movecolums diminue le nb de boite ds lowbarrelCount et donc on finit pas la boucle et fait pas spinbarrel
        return false;
    }

    
    return true;
}

// -------------------------------------------------
// Functions to handle outgoing
// -------------------------------------------------

bool RevolverRelease(){
    LOG_INFO("RevolverRelease"); //prepare release low barrel
    return ReleaseLow();
}

// Function to release all columns from the barrel
bool ReleaseLow() {
    LOG_INFO("ReleaseLow");
    
    if (SpinLowBarrel(ShiftListNumber(lowArr, 3, false))){
        DisplayBarrel();
        if (lowBarrelCount == 0) { 
            LOG_INFO("No columns to release");
        } 
        else if (lowArr[2] && lowArr[3]) {
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

void TestTakeAction(direction_t dir){
    if (isRevolverFull())
        return;
    while (!RevolverPrepareLowBarrel(dir));
    while (!RevolverLoadStock(dir));
    DisplayBarrel();
}

void TestRevolver(){
    DisplayRobot();
    DisplayBarrel();

    emulateActuators = true;

    TestTakeAction(FROM_RIGHT);
    TestTakeAction(FROM_LEFT);
    TestTakeAction(FROM_RIGHT);
    TestTakeAction(FROM_LEFT);
    TestTakeAction(FROM_RIGHT);
    TestTakeAction(FROM_LEFT);
    while (!isRevolverEmpty())
        RevolverRelease();

    emulateActuators = false;
    
    LOG_GREEN_INFO("Done Test Revolver !");
    
}