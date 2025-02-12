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

    printf("Barillet1: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", lowBarrelTab[i]);
    }
    printf("]\n Barillet2: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", highBarrelTab[i]);
    }
    printf("]\n\n");
}

void DisplayBarrel(){}
void SpinBarrel(){}
void MoveUpcolumns(){}
void PrerareBarrel(){}
void LoadStock(){}