#include "actions/revolver.hpp"
#include "utils/logger.hpp"
#include "actions/functions.h"
#define SIZE 14

bool lowBarrelTab[SIZE] = {0};  // false = Empty,  true = Occupied
bool highBarrelTab[SIZE] = {0}; // false = Empty,  true = Occupied
int lowBarrelCount = 0, highBarrelCount = 0;

// Internal prototypes
void initRevolver();
void DisplayRobot();
void DisplayBarrel();
bool SpinBarrel(int n, int num_tab);
bool MoveColumns(int direction, int sens);
bool PrerareLowBarrel(direction_t dir);
bool PrerareHighBarrel(direction_t dir);
bool PrepareHighBarrel(direction_t dir);
bool LoadStock(direction_t dir);
bool ReleaseLow();
bool ReleaseHigh();


// Initialize the revolver when the game starts
void initRevolver(){
    for (int i = 0; i < SIZE; i++) {
        lowBarrelTab[i] = highBarrelTab[i] = 0;
    }
    lowBarrelCount = highBarrelCount = 0;
}

// -------------------------------------------------
// Display functions
// -------------------------------------------------

// Display the robot structure and the layout of the locations
void DisplayRobot(){
    printf("Affichage Robot \n");
    printf("Etage 1 :                etage2 :\n");
    printf(" 1 2 3 4                 1 2 3 4\n");
    printf("0       5               0        5\n");
    printf("13      6               13       6\n");
    printf("12      7               12       7\n");
    printf("11 10 9 8               11 10 9 8\n\n");
}

// Display the current state of the two barrels in circular and linear form
void DisplayBarrel(){
    printf(" %d %d %d %d               %d %d %d %d\n", lowBarrelTab[1], lowBarrelTab[2], lowBarrelTab[3], lowBarrelTab[4],-1,-1,-1,-1);
    printf("%d        %d             %d        %d\n", lowBarrelTab[0], lowBarrelTab[5],highBarrelTab[0], highBarrelTab[5]);
    printf("%d        %d             %d        %d\n", lowBarrelTab[13], lowBarrelTab[6],highBarrelTab[13], highBarrelTab[6]);
    printf("%d        %d             %d        %d\n", lowBarrelTab[12], lowBarrelTab[7],highBarrelTab[12], highBarrelTab[7]);
    printf(" %d %d %d %d                %d %d %d %d\n", lowBarrelTab[11], lowBarrelTab[10], lowBarrelTab[9], lowBarrelTab[8],highBarrelTab[11], highBarrelTab[10], highBarrelTab[9], highBarrelTab[8]);
    printf("\n\n");
}

// -------------------------------------------------
// Functions to handle revolver
// -------------------------------------------------

// Spin the barrel by n positions (positive or negative) returns true when done
bool SpinBarrel(int n, int num_tab) {//lowBarrel 1er = 1: highBarrel 2eme = 2
    LOG_INFO("SpinBarrel n=", n, " tab=", num_tab, (n > 0) ? " Clockwise" : " Anticlockwise");
    bool *lowBarrel_actuel = (num_tab == 1) ? lowBarrelTab : highBarrelTab;
    int temp[SIZE];
    for (int i = 0; i < SIZE; i++)
        temp[(i + n + SIZE) % SIZE] = lowBarrel_actuel[i];
    for (int i = 0; i < SIZE; i++) {
        if (!(num_tab == 2 && i >= 1 && i <= 4))
            lowBarrel_actuel[i] = temp[i];
    }
    for (int i = 1; i <= 4; i++) { // Case 1 2 3 4 2eme étage impossible
        if (temp[i] == 1 && num_tab == 2) LOG_ERROR("Placement interdit");
    }
    DisplayBarrel();
    //MoveStepper(n, num_tab);
    return 1; // when finished
}

//return the shift needed to put first or last 1 to desired position
int ShiftListNumber(bool list[], int desired_position, int choose_first) {//choose_first = 1 pour mettre le premier 1, 0 pour mettre le dernier 1
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

    if (sens == 1 && highBarrelTab[start]) {
        LOG_ERROR("T'essais de monter des columns de conserve ou il y en a deja");
        // TODO Maybe return error or throw ?
    }

    highBarrelTab[start] = highBarrelTab[end] = (sens == 1) ? 1 : 0;;
    lowBarrelTab[start] = lowBarrelTab[end] = (sens == 1) ? 0 : 1;;
    //if (!moveServoFloorColumns(sens)) return 0;
    highBarrelCount += sens ? 2 : -2;
    lowBarrelCount -= sens ? 2 : -2;
    DisplayBarrel();
    return true;
}

// -------------------------------------------------
// Functions to handle incomming stock
// -------------------------------------------------

// Load a Stock according to the direction
bool LoadStock(direction_t dir){
    LOG_INFO("INFO : AjoutColumn ", (dir == FROM_RIGHT) ? "Right" : "Left");
    int position = (dir == FROM_RIGHT) ? 5 : 0;
    int rotation = (dir == FROM_RIGHT) ? -1 : 1; // position ajout column, Sens de rotation
    
    for (int i = 0; i < 4; i++) {
        LOG_INFO("ajout column pos ", abs(position-1-i));
        lowBarrelTab[position] = 1; // Remplit 4 cases successives
        lowBarrelCount++;
        SpinBarrel(rotation, 1);
    }
    return true; // TODO
}

// Function that manages the storage of the second level. Returns true when done.
bool PrepareHighBarrel(direction_t dir){
    LOG_INFO("PrepareHighBarrel sens : ", dir, "\n");
    if (highBarrelCount == 0)return 1; //no columns in Highbarrel so position is good
    if (highBarrelCount >= 10) {LOG_ERROR("Plus de place dans le barillet 2"); return 1;}

    if (!SpinBarrel(ShiftListNumber(highBarrelTab, (dir==FROM_RIGHT) ? 7 : 12, dir),2)) return 0; //n = shift needed to put first or last 1 to desired position (7 or 12)
    return 1;
}

//Fonction qui Gère le stockage du 1er étage de columns, 
bool PrerareLowBarrel(direction_t dir){
    LOG_INFO("PrerareLowBarrel sens : ",((dir==FROM_RIGHT) ? "droite" : "gauche"));
    if (lowBarrelCount == 0) return 1; //no columns in Lowbarrel so position is good

    if (!SpinBarrel(ShiftListNumber(lowBarrelTab, dir ? 4 : 1, !dir),1)) return false;
    PrepareHighBarrel(dir);
    int N = (dir == FROM_RIGHT) ? 2 : -2;
    if(lowBarrelCount == 12) {
        if (!SpinBarrel(N, 1))
            return false;
        MoveColumns(dir, 1);      //si on fait preparelowbarrel en boucle, movecolums diminue le nb de boite ds lowbarrelCount et donc on finit pas la boucle et fait pas spinbarrel
        if (highBarrelCount <= 8) 
            SpinBarrel(N, 2);
        return false;
    }
    if (lowBarrelCount == SIZE){
        MoveColumns(dir, 1);
        if (highBarrelCount <= 8) 
            SpinBarrel(N, 2);
        return false;
    }
    
    return true;
}

// -------------------------------------------------
// Functions to handle outgoing
// -------------------------------------------------

bool Release(){
    LOG_INFO("Release"); //prepare release low barrel
    if (!SpinBarrel(ShiftListNumber(lowBarrelTab,3,0),1));
    if (!SpinBarrel(ShiftListNumber(highBarrelTab, 0, 0),2));
    DisplayBarrel();
    if (!ReleaseLow()) return 0;
    return 1;
}

bool ReleaseHigh(){
    LOG_INFO("ReleaseHigh");
    if (highBarrelCount == 0) {LOG_INFO("Plus de columns a sortir 2eme etage"); return 1;}
    
    highBarrelTab[0] = highBarrelTab[13] = 0;//baisser les columns
    highBarrelCount -= 2;
    SpinBarrel(2,2);
    SpinBarrel(3,1);
    DisplayBarrel();
    return 0;
}

// Function to release all columns from the barrel
bool ReleaseLow() {
    LOG_INFO("ReleaseLow");
    if (lowBarrelCount == 0) { 
        LOG_INFO("No columns to release from the first stage");
        return ReleaseHigh();
    } 
    lowBarrelTab[2] = 0;
    lowBarrelTab[3] = 0;
    lowBarrelCount -= 2;
    SpinBarrel(2, 1);
    DisplayBarrel();
    return false;
}

// -------------------------------------------------
// Test Functions
// -------------------------------------------------

void TestTakeAction(direction_t dir){
    if (lowBarrelCount == SIZE && highBarrelCount == 10) {LOG_ERROR("Plus de place dans le revolver");return;}
    while (!PrerareLowBarrel(dir));
    LoadStock(dir);
    DisplayBarrel();
}

void TestRevolver(){
    DisplayRobot();
    DisplayBarrel();

    TestTakeAction(FROM_RIGHT);
    TestTakeAction(FROM_LEFT);
    TestTakeAction(FROM_RIGHT);
    TestTakeAction(FROM_LEFT);
    TestTakeAction(FROM_RIGHT);
    TestTakeAction(FROM_LEFT);
    while (!Release());

    LOG_GREEN_INFO("Done Test Revolver !");
    
}