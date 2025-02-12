#include "revolver.h"

int lowBarrelTab[SIZE] = {0};  // 0 = vide, 1 = occupé
int highBarrelTab[SIZE] = {0};  // 0 = vide, 1 = occupé
int lowBarrelCount=0, highBarrelCount = 0;

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

    printf("lowBarrel1: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", lowBarrelTab[i]);
    }
    printf("]\n highBarrel: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", highBarrelTab[i]);
    }
    printf("]\n\n");
}

//Spin the barrel by n positions (positive or negative)
void SpinBarrel(int n, int num_tab) {//lowBarrel 1er = 1: highBarrel 2ème = 2
    int *lowBarrel_actuel = (num_tab == 1) ? lowBarrelTab : highBarrelTab;
    int temp[SIZE];
    
    for (int i = 0; i < SIZE; i++) {
        temp[(i + n + SIZE) % SIZE] = lowBarrel_actuel[i];
    }
    
    for (int i = 0; i < SIZE; i++) {
    if (!(num_tab == 2 && i >= 1 && i <= 4)){// && temp[i] != -1 inutile ?
        lowBarrel_actuel[i] = temp[i];
        }
    }
    
    for (int i = 1; i <= 4; i++) {
        if (temp[i] == 1 && num_tab == 2) {//case 1 2 3 4 du tab 2 impossible
            printf("T'essais de mettre des boite de conserve à un endroit interdit\n");
        }
    }
    //TurnBarrel(n); //TODO implémenter ds functions
}

void MoveUpColumns(int direction){
    int positions[2] = { direction == 0 ? 0 : 5, direction == 0 ? 13 : 6};
    highBarrelTab[positions[0]] = 1; highBarrelTab[positions[1]] = 1; // Monter les boîtes
    lowBarrelTab[positions[0]] = 0; lowBarrelTab[positions[1]] = 0;
    //ArduinoMoveColums(1 ou 0) //TODO implémenter ds functions
}

//Fonction qui Gère le stockage du 1er étage de boîtes
void PrerareLowBarrel(){}

//Fonction qui Gère le stockage du 2ème étage de boîtes
void PrepareHighBarrel(){}

//Load a Stock according to the direction (right = 1, left = 0) (droite = 1, gauche = 0)
void LoadStock(int direction){
    printf("AjoutBoite%s\n", direction == 1 ? "Droite" : "Gauche");
    if (lowBarrelCount > 10) { 
        if (10- highBarrelCount < 4){ printf("PLUS DE PLACE !!\nPLUS DE PLACE !!\nPLUS DE PLACE !!\n\n");}
        monterEtage((direction == 1) ? 1 : 0);
    }
    
    int position = (direction == 1) ? 5 : 0; // Position initiale selon la direction
    int rotation = (direction == 1) ? -1 : 1; // Sens de rotation
    
    while (lowBarrelTab[position] == 1 && lowBarrelCount <= 14) {
        SpinBarrel(rotation, 1); // Cherche un emplacement libre 
    }
    
    for (int i = 0; i < 4; i++) {
        lowBarrelTab[position] = 1; // Remplit 4 cases successives
        lowBarrelCount++;
        SpinBarrel(rotation, 1);
    }
    DisplayRobot();
    printf("\n");
}

void TestRevolver(){
    DisplayRobot();
    DisplayBarrel();
    LoadStock(1);
    LoadStock(0);
    LoadStock(1);

    DisplayBarrel();
}