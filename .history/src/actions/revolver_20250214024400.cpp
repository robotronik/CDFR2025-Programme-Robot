#include "actions/revolver.hpp"
#include "utils/logger.hpp"
#include "actions/functions.h"

int lowBarrelTab[SIZE] = {0};  // 0 = Empty, 1 = Occupied
int highBarrelTab[SIZE] = {0};  // 0 = Empty, 1 = Occupied
int lowBarrelCount = 0, highBarrelCount = 0;

// Initialize the revolver when the game starts
void initRevolver(){
    for (int i = 0; i < SIZE; i++) {
        lowBarrelTab[i] = highBarrelTab[i] = 0;
    }
    lowBarrelCount = highBarrelCount = 0;
}

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

// Spin the barrel by n positions (positive or negative) returns true when done
bool SpinBarrel(int n, int num_tab) {//lowBarrel 1er = 1: highBarrel 2ème = 2
    //LOG_INFO("SpinBarrel n =", n, "tab = ", num_tab, (n > 0) ? " sens horaire" : " sens anti-horaire");
    int *lowBarrel_actuel = (num_tab == 1) ? lowBarrelTab : highBarrelTab;
    int temp[SIZE];
    for (int i = 0; i < SIZE; i++) {temp[(i + n + SIZE) % SIZE] = lowBarrel_actuel[i];    }
    for (int i = 0; i < SIZE; i++) {if (!(num_tab == 2 && i >= 1 && i <= 4)){lowBarrel_actuel[i] = temp[i];}}
    for (int i = 1; i <= 4; i++) {if (temp[i] == 1 && num_tab == 2) {LOG_ERROR("Placement interdit");}}//case 1 2 3 4 2ème étage impossible
    return 1; // when finished
}

//return the shift needed to put first or last 1 to desired position
int ShiftListNumber(int list[], int desired_position, int choose_first) {//choose_first = 1 pour mettre le premier 1, 0 pour mettre le dernier 1
    int n = 14, first = -1, last = -1;
    for (int i = 0; i < n; i++) {
        if (list[i] == 1 && list[(i - 1 + n) % n] == 0) first = (first == -1) ? i : first;
        if (list[i] == 1 && list[(i + 1) % n] == 0) last = i;
    }
    if (first == -1) return 0;
    int shift = (desired_position - (choose_first ? first : last)) % n;
    return (shift > n / 2) ? shift - n : shift;
}
    

// Returns true when done
bool MoveColumns(int direction, int sens) { //return 1 when finished
    LOG_INFO("MoveColumns direction : ", direction, " sens : ", sens);
    int start = (direction == 0 ? 0 : 5),  end = (direction == 0 ? 13 : 6);

    if (sens == 1 && highBarrelTab[start]) {
        LOG_ERROR("T'essais de monter des columns de conserve ou il y en a deja\n");
        // TODO Maybe return error or throw ?
    }

    highBarrelTab[start] = highBarrelTab[end] = (sens == 1) ? 1 : 0;;
    lowBarrelTab[start] = lowBarrelTab[end] = (sens == 1) ? 0 : 1;;
    highBarrelCount += sens ? 2 : -2;
    lowBarrelCount -= sens ? 2 : -2; // TODO si MoveColumns est appeler jusqu'a que l'action soit finit, ça décrémente lowBarrelCount de 2 à chaque fois ???
    return true;
    // TODO Add more logic to handle spinning by +N (?? rien ne spin ici)
}


//Fonction qui Gère le stockage du 2ème étage de boîtes returns true when done
bool PrepareHighBarrel(int sens){//sens 1 = droite, 0 = gauche
    LOG_INFO("PrepareHighBarrel sens : ", sens, "\n");
    if (highBarrelCount == 0)return 1; //no columns in Highbarrel so position is good
    if (highBarrelCount >= 10) {LOG_ERROR("Plus de place dans le barillet 2"); return 1;}

    if (!SpinBarrel(ShiftListNumber(highBarrelTab, sens ? 7 : 12, sens),2)) return 0; //n = shift needed to put first or last 1 to desired position (7 or 12)
    return 1;
}

//Fonction qui Gère le stockage du 1er étage de boîtes, 
bool PrerareLowBarrel(int sens){//sens 1 = droite, 0 = gauche
    LOG_INFO("PrerareLowBarrel sens : ",(sens ? "droite" : "gauche"));
    if (lowBarrelCount == 0) return 1; //no columns in Lowbarrel so position is good

    if (!SpinBarrel(ShiftListNumber(lowBarrelTab, sens ? 4 : 1, !sens),1)) return 0;

    PrepareHighBarrel(sens);
    if(lowBarrelCount == 12) {
        SpinBarrel(sens ? 2 : -2, 1); //comment return 0 ??? quand c'est fini, il va relancer la fonction 
        MoveColumns(sens, 1);      //si on fait preparelowbarrel en boucle, movecolums diminue le nb de boite ds lowbarrelCount et donc on finit pas la boucle et fait pas spinbarrel
        if (highBarrelCount<=8) SpinBarrel(sens ? 2 : -2, 2);
        return 0;
    }
    if (lowBarrelCount == SIZE){
        MoveColumns(sens, 1);        //comment return 0 ???
        if (highBarrelCount<=8) SpinBarrel(sens ? 2 : -2, 2);
        return 0;
    }
    return 1;
}


// Load a Stock according to the direction (right = 1, left = 0) (droite = 1, gauche = 0)
bool LoadStock(int direction){
    LOG_INFO("INFO : AjoutColumn", direction == 1 ? "Droite" : "Gauche");
    int position = direction ? 5 : 0, rotation = direction ? -1 : 1; // position ajout column, Sens de rotation
    
    for (int i = 0; i < 4; i++) {
        LOG_INFO("ajout column pos ", abs(position-1-i));
        lowBarrelTab[position] = 1; // Remplit 4 cases successives
        lowBarrelCount++;
        SpinBarrel(rotation, 1);
    }
    return true; // TODO
}

void take(int sens){//sens 1 = droite, 0 = gauche
    if (lowBarrelCount == SIZE && highBarrelCount == 10) {LOG_ERROR("Plus de place dans le revolver");return;}
    while (!PrerareLowBarrel(sens));
    LoadStock(sens);
    DisplayBarrel();
}

bool ReleaseHigh(){
    LOG_INFO("ReleaseHigh");
    if (highBarrelCount == 0) {LOG_INFO("Plus de columns a sortir 2eme etage"); return 1;}
    
    highBarrelTab[0] = highBarrelTab[13] = 0;//baisser les boîtes
    highBarrelCount -= 2;
    SpinBarrel(2,2);
    SpinBarrel(3,1);
    DisplayBarrel();
    return 0;
}

//fonction qui sort tout les boîtes du barillet
bool ReleaseLow(){//sens 1 = droite, 0 = gauche
    LOG_INFO("ReleaseOne");
    if (lowBarrelCount == 0) {LOG_INFO("Plus de columns a sortir 1er etage"); return ReleaseHigh();} 
    
    lowBarrelTab[2] = 0; lowBarrelTab[3] = 0;
    lowBarrelCount -= 2;
    SpinBarrel(2,1);
    DisplayBarrel();
    return 0;
}

bool Release(){
    LOG_INFO("PrepareRelease"); //prepare release low barrel
    if (!SpinBarrel(ShiftListNumber(lowBarrelTab,3,0),1));
    if (!SpinBarrel(ShiftListNumber(highBarrelTab, 0, 0),2));
    DisplayBarrel();
    if (!ReleaseLow()) return 0;
    return 1;
}


void TestRevolver(){
    DisplayRobot();
    DisplayBarrel();

    take(1);
    take(0);
    take(1);
    take(0);
    take(1);
    take(0);
    while (!Release());

    LOG_GREEN_INFO("Done Test Revolver !");
    
}