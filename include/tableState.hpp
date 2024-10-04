#pragma once
#include "structs.hpp"
#include "affichage.hpp"
#include "logger.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef struct 
{
    bool etat; // si l'action a été faite ou non
    int cout; // points que rapporte l'action
    float tps; //temps passé par l'ennemie dans l'action
    colorTeam_t color; //NONE, BLUE or YELLOW
}table_t;

// Define serialization for table_t
void to_json(json& j, const table_t& t);

class TableState
{
public:

    TableState();
    ~TableState();
    void init(Affichage* affichage);

    int getScore();
    void setScore(int score);
    void incrementScore(int score);
    
    /* data generale*/
    int dx,dy;                //Faut que quelquun me dise ce que c'est ça
    position_t ennemie;
    position_t prev_pos;
    position_t init_pos; 
    int nb;                   //Et ça
    unsigned long startTime;
    bool FIN;
    robot_t robot;
    table_t zoneFull[6];          // zone départ/arrivés

    /* data show must go on*/
    table_t stock[10];
    table_t banderole;

private:
    int score;
    Affichage* affichage;
};

// Serialize tableState
void to_json(json& j, const TableState& ts);