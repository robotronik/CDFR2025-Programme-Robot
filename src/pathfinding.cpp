#include "pathfinding.hpp"

int getOrientation(int x, int y){
    return (int)((x/ abs(x) == 1 ? 0 : 1*M_PI) -  atan(x / y)/DEG_TO_RAD);
}

float distance2(position_t p1, position_t p2){
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

instruction_t stack_pop(Astack* stack, position_t* parent){
    while(!stack->empty() && &(stack->top()) != parent){
        stack->pop();
    }
    if(stack->empty()){
        perror("Stack vide");
    }
    else{
        instruction_t previous = stack->top();
        stack->pop();
        return previous;
    }
}

void RobotAdverse_obstacle(tableState* itable, float pastemps, int numpas ){
    robot_t* adversaire = itable->adversaire;
    obstacle_t *obstacle = adversaire->obstacle;

    obstacle->forme = RECTANGLE;
    obstacle->demie_largeur = LARGEUR_ROBOT*SECURITE/2;
    obstacle->demie_longueur = sqrt(pow(adversaire->vit_x,2) + pow(adversaire->vit_y,2))*pastemps* SECURITE /2;
    obstacle->teta = (adversaire->pos.teta);
    obstacle->centre_x = adversaire->pos.x + adversaire->vit_x*pastemps*SECURITE/2;
    obstacle->centre_y = adversaire->pos.y + adversaire->vit_y*pastemps*SECURITE/2;
    return;
}

void RobotActualise(robot_t* robot, int x, int y , int time){
    robot->vit_x = (x - robot->pos.x)/(time-robot->pos.time);
    robot->vit_y = (y - robot->pos.y)/(time-robot->pos.time);
    robot->pos.teta = getOrientation(robot->vit_x,robot->vit_y);
    robot->pos.x= x;
    robot->pos.y= y;
    robot->pos.time= time;
    return;
}

int RobotGetZone(position_t pos, int N){
    int zone = pos.x * 2 + N*2* pos.y;
    return zone;
}

instruction_t Euristique(position_t current, position_t* objectif){
    instruction_t next;
    next.parent = current;


    return next;
}


instruction_t* A_star(tableState* itable, position_t objectif){
    heap Calculer;
    Astack traiter;
    position_t currentPos = itable->nous->pos;

    CalculeNext(&Calculer,objectif,currentPos);
    int zone = RobotGetZone(currentPos, itable->dimensionZone);
    
}


void CalculeNext(heap* Calculer, position_t objectif, position_t currentPos){
    position_t nextPos = currentPos;
    int tetaIdeal = getOrientation(objectif.x-currentPos.x,objectif.y-currentPos.y);
    int MouvDist = 0;
    int tetaMouv = 0;
    for (int i = 0; i < NOMBRE_TEST_Angle; i++)
    {
        nextPos.teta = tetaIdeal + i* ( 360/(NOMBRE_TEST_Angle-2));
        tetaMouv = nextPos.teta-currentPos.teta;
        *Calculer.push(Euristique(nextPos,objectif,tetaMouv,MouvDist));
    }

    nextPos = itable->nous->pos;
    tetaMouv = 0;
    for (int i = 1; i <= NOMBRE_TEST_DROIT; i++)
    {
        MouvDist = (int)(distance2(objectif,currentPos)>200 ? 200:distance2(objectif,currentPos) )/i ;
        *Calculer.push(Euristique(nextPos,objectif,tetaMouv,MouvDist));
        *Calculer.push(Euristique(nextPos,objectif,tetaMouv,-MouvDist));
    }
}