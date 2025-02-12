#include "revolver.h"

int lowBarrelTab[SIZE] = {0};  // 0 = vide, 1 = occupé
int highBarrelTab[SIZE] = {0};  // 0 = vide, 1 = occupé
int lowBarrelCount=0, highBarrelCount = 0;
void DisplayRobot(){
    printf("Affichage Robot \n");
    printf("Etage 1 :                etage2 :\n");
    printf(" 1 2 3 4                 1 2 3 4\n");
    printf("0       5               0        5\n");
    printf("13      6               13       6\n");
    printf("12      7               12       7\n");
    printf("11 10 9 8               11 10 9 8\n\n");
}

void DisplayRobot(){
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

void tournerBarrel(int n, int num_tab) {//lowBarrel 1er = 1: highBarrel 2ème = 2
    int *lowBarrel_actuel = (num_tab == 1) ? lowBarrelTab : highBarrelTab;
    int temp[SIZE];
    
    for (int i = 0; i < SIZE; i++) {
        temp[(i + (n + SIZE) % SIZE) % SIZE] = lowBarrel_actuel[i];
    }
    
    for (int i = 0; i < SIZE; i++) {
    if (!(num_tab == 2 && i >= 1 && i <= 4) && temp[i] != -1) {
        lowBarrel_actuel[i] = temp[i];
        }
    }
    
    for (int i = 1; i <= 4; i++) {
        if (temp[i] == 1 && num_tab == 2) {//case 1 2 3 4 du tab 2 impossible
            printf("T'essais de mettre des boite de conserve à un endroit interdit\n");
        }
    }
    TurnBarrel(n); //TODO implémenter ds functions
}

void SpinBarrel(){}
void MoveUpcolumns(){}
void PrerareBarrel(){}
void LoadStock(){}