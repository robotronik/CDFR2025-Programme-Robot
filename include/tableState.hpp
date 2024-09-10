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

    TableState(Affichage& affichage);
    ~TableState();
    void init();
    /* data */
    table_t planteStockFull[6]; //zonne des plantes
    table_t JardiniereFull[6]; //zone des jardinières
    table_t zoneFull[6];          // zone départ/arrivés
    table_t jardiniereFree[6];    //zonne devant les jardinières
    table_t panneauSolaireRotate[9];
    int dx,dy;
    int getScore();
    void setScore(int score);
    void incrementScore(int score);
    position_t ennemie;
    position_t prev_pos;
    position_t init_pos; int nb;
    table_t solarPanelTurn;
    unsigned long startTime;
    bool FIN;
    robot_t robot;

private:
    int score;
    Affichage& affichage;

};

// Serialize tableState
void to_json(json& j, const TableState& ts);