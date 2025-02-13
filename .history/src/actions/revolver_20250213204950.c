#include "revolver.hpp"


int lowBarrelTab[SIZE] = {0};  // 0 = vide, 1 = occupé
int highBarrelTab[SIZE] = {0};  // 0 = vide, 1 = occupé
int lowBarrelCount=0, highBarrelCount = 0;

void initRevolver(){//initialise les barillets si on re-initialise le programme
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

//Spin the barrel by n positions (positive or negative) return 1 when finished
bool SpinBarrel(int n, int num_tab) {//lowBarrel 1er = 1: highBarrel 2ème = 2
    printf("INFO : SpinBarrel num %i , %s \n", num_tab, (n > 0) ? "sens horaire" : "sens anti-horaire");
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
    
    for (int i = 1; i <= 4; i++) {//case 1 2 3 4 2ème étage impossible
        if (temp[i] == 1 && num_tab == 2) {
            printf("ERREUR : Placement interdit\n");
        }
    }
    //return TurnBarrel(n); //TODO implémenter ds functions (13 teeths)
    return 1;
}

bool MoveColumns(int direction, int sens) { //return 1 when finished
    printf("INFO : MoveColumns direction : %i, sens : %i\n", direction, sens);
    int start = (direction == 0 ? 0 : 5), end = (direction == 0 ? 13 : 6);

    if (sens == 1 && highBarrelTab[start]) {printf("ERREUR : T'essais de monter des boîtes de conserve où il y en a déjà\n");}

    highBarrelTab[start] = highBarrelTab[end] = (sens == 1) ? 1 : 0;;
    lowBarrelTab[start] = lowBarrelTab[end] = (sens == 1) ? 0 : 1;;
    highBarrelCount += sens ? 2 : -2;
    lowBarrelCount -= sens ? 2 : -2;
    //return ArduinoMoveColumns()
    return true; // TODO: Remplacer par ArduinoMoveColumns()
}


//Fonction qui Gère le stockage du 2ème étage de boîtes return 1 when finished
bool PrepareHighBarrel(int sens){//sens 1 = droite, 0 = gauche
    printf("INFO : PrepareHighBarrel sens : %i\n", sens);
    if (highBarrelCount == 0) return 1; //no columns in Highbarrel so position is good
    if (highBarrelCount >= 10) {printf("ERREUR Plus de place dans le barillet 2\n"); return 1;}
    
    if (highBarrelTab[sens ? 5 : 0]) {
        SpinBarrel(sens ? 2 : -2, 2); 
        return 1; //Une rotation de 2 suffit
    }

    if (highBarrelCount <= 8 && !highBarrelTab[sens ? 7 : 12]){//whille there is not a box in the last column (7 or 12)
        SpinBarrel(sens ? -2 : 2, 2);
        return 0; 
    }
    return 1;
}

//Fonction qui Gère le stockage du 1er étage de boîtes, 
bool PrerareLowBarrel(int sens){//sens 1 = droite, 0 = gauche
    printf("INFO : PrerareLowBarrel sens : %s\n",(sens ? "droite" : "gauche"));
    if (lowBarrelCount == 0) return 1; //no columns in Lowbarrel so position is good

    if (lowBarrelCount <= 10){
        if (lowBarrelTab[sens ? 5 : 0] != 0) {//while there is not a box in the last column (5 or 0)
            SpinBarrel(sens ? -2 : 2, 1);
            return 0; 
        }
        return 1; //Barrel placed
    }
    if (lowBarrelCount == 12 && lowBarrelTab[sens ? 8 : 11] != 0) {
        SpinBarrel(sens ? 2 : -2, 1);
        return 0;
    }

    if (lowBarrelCount == SIZE) {//call PrepareHighBarrel when first stage barrel is full
        if (!PrepareHighBarrel(sens)) return 0;
        MoveColumns(sens, 1);
        return 0;
    }

    if (!PrepareHighBarrel(sens)) return 0;
    printf("\ntest : %i\n\n", lowBarrelCount);
    MoveColumns(sens, 1);
    return PrepareHighBarrel(sens);
}


//Load a Stock according to the direction (right = 1, left = 0) (droite = 1, gauche = 0)
bool LoadStock(int direction){
    printf("INFO : AjoutBoite%s\n", direction == 1 ? "Droite" : "Gauche");
    int position = direction ? 5 : 0, rotation = direction ? -1 : 1; // position ajout boite, Sens de rotation
    
    for (int i = 0; i < 4; i++) {
        printf("INFO : ajout boite pos %i\n", abs(position-1-i));
        lowBarrelTab[position] = 1; // Remplit 4 cases successives
        lowBarrelCount++;
        SpinBarrel(rotation, 1);
    }
}

void take(int sens){//sens 1 = droite, 0 = gauche
    if (lowBarrelCount == SIZE && highBarrelCount == 10) {printf("ERREUR : Plus de place dans le revolver \n"); return;}
    while (!PrerareLowBarrel(sens));
    LoadStock(sens);
    DisplayBarrel();
}

bool PrepareRelease(){
    printf("PrepareRelease\n"); //prepare release low barrel
    if (lowBarrelTab[4] == 1 && lowBarrelTab[5] == 0){ //wait for the first box to be in the right position
        SpinBarrel(-1, 1);
        return 0;
    }
    if (!(lowBarrelTab[4] == 0 && lowBarrelTab[3] == 1) && lowBarrelCount != SIZE){ //
        SpinBarrel(1, 1);
        return 0;
    }

    //prepare release high barrel
    if (highBarrelTab[0] == 0){
        SpinBarrel(1, 2);
        return 0;
    }
    DisplayBarrel();
    return 1;
}
bool ReleaseHigh(){printf("INFO : ReleaseHigh\n");
    if (highBarrelCount == 0) {printf("INFO : Plus de boîtes à sortir 2ème étage\n"); return 1;}
    
    highBarrelTab[0] = highBarrelTab[13] = 0;//baisser les boîtes
    highBarrelCount -= 2;
    SpinBarrel(2,2);
    SpinBarrel(3,1);
    DisplayBarrel();
    return 0;
}

//fonction qui sort tout les boîtes du barillet
bool ReleaseLow(){//sens 1 = droite, 0 = gauche
    printf("INFO : ReleaseOne\n");
    if (lowBarrelCount == 0) {printf("INFO : Plus de boîtes à sortir 1er etage\n"); 
        return ReleaseHigh();
    }
    lowBarrelTab[2] = 0; lowBarrelTab[3] = 0;
    lowBarrelCount -= 2;
    SpinBarrel(2,1);
    DisplayBarrel();
    return 0;
}

void main(){
    DisplayRobot();
    DisplayBarrel();


    take(0);
    take(1);
    take(1);
    take(1);
    take(1);
    while (!PrepareRelease());
    while (!ReleaseLow());

    printf("\nTerminé !\n");
    
}