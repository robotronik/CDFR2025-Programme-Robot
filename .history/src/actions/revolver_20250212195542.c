#include "revolver.h"

int barillet[SIZE] = {0};  // 0 = vide, 1 = occupé
int barillet2[SIZE] = {0};  // 0 = vide, 1 = occupé
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
    printf(" %d %d %d %d               %d %d %d %d\n", barillet[1], barillet[2], barillet[3], barillet[4],-1,-1,-1,-1);
    printf("%d        %d             %d        %d\n", barillet[0], barillet[5],barillet2[0], barillet2[5]);
    printf("%d        %d             %d        %d\n", barillet[13], barillet[6],barillet2[13], barillet2[6]);
    printf("%d        %d             %d        %d\n", barillet[12], barillet[7],barillet2[12], barillet2[7]);
    printf(" %d %d %d %d                %d %d %d %d\n", barillet[11], barillet[10], barillet[9], barillet[8],barillet2[11], barillet2[10], barillet2[9], barillet2[8]);

    printf("Barillet1: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", barillet[i]);
    }
    printf("]\n Barillet2: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", barillet2[i]);
    }
    printf("]\n\n");
}

void DisplayBarrel(){}
void SpinBarrel(){}
void MoveUpcolumns(){}
void PrerareBarrel(){}
void LoadStock(){}