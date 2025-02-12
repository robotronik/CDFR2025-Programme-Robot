#include "revolver.h"

int lowBarrel[SIZE] = {0};  // 0 = vide, 1 = occupé
int highBarrel[SIZE] = {0};  // 0 = vide, 1 = occupé
int nb_boite=0, nb_boite2 = 0;

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
void DisplayRobot(){
    printf(" %d %d %d %d               %d %d %d %d\n", lowBarrel[1], lowBarrel[2], lowBarrel[3], lowBarrel[4],-1,-1,-1,-1);
    printf("%d        %d             %d        %d\n", lowBarrel[0], lowBarrel[5],highBarrel[0], highBarrel[5]);
    printf("%d        %d             %d        %d\n", lowBarrel[13], lowBarrel[6],highBarrel[13], highBarrel[6]);
    printf("%d        %d             %d        %d\n", lowBarrel[12], lowBarrel[7],highBarrel[12], highBarrel[7]);
    printf(" %d %d %d %d                %d %d %d %d\n", lowBarrel[11], lowBarrel[10], lowBarrel[9], lowBarrel[8],highBarrel[11], highBarrel[10], highBarrel[9], highBarrel[8]);

    printf("lowBarrel1: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", lowBarrel[i]);
    }
    printf("]\n highBarrel: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", highBarrel[i]);
    }
    printf("]\n\n");
}

//Spin the barrel by n positions (positive or negative)
void SpinBarrel(int n, int num_tab) {//lowBarrel 1er = 1: highBarrel 2ème = 2
    int *lowBarrel_actuel = (num_tab == 1) ? lowBarrel : highBarrel;
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

void MoveUpcolumns(){}

void PrerareBarrel(){}

void LoadStock(){
    int SpinBarrel(int direction) {// Ajoute une boîte en fonction de la direction (droite = 1, gauche = 0)
    printf("AjoutBoite%s\n", direction == 1 ? "Droite" : "Gauche");
    if (nb_boite > 10) { 
        if (10- nb_boite2 < 4){ printf("PLUS DE PLACE !!\nPLUS DE PLACE !!\nPLUS DE PLACE !!\n\n");}
        monterEtage((direction == 1) ? 1 : 0);
    }
    
    int position = (direction == 1) ? 5 : 0; // Position initiale selon la direction
    int rotation = (direction == 1) ? -1 : 1; // Sens de rotation
    
    while (barillet[position] == 1 && nb_boite <= 14) {
        tournerBarillet(rotation, 1); // Cherche un emplacement libre 
    }
    
    for (int i = 0; i < 4; i++) {
        barillet[position] = 1; // Remplit 4 cases successives
        nb_boite++;
        tournerBarillet(rotation, 1);
    }
    afficherBarillet();
    printf("\n");
}