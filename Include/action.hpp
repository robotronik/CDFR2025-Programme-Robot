#pragma once

#include <functional>
#include <string>
#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "robot.h"
#include "tableState.hpp"

class action;

typedef int (*FuncRunPtr)(action*, robotCDFR*, Asser*, Arduino*, tableState*);
typedef int (*FuncValidPtr)(tableState*);


class action
{
private: 


    typedef enum{
        FSMACTION_INIT,
        FSMACTION_MOVESTART,
        FSMACTION_ACTION,
        FSMACTION_MOVEEND
    }fsmAction_t;

private:
    robotCDFR* mainRobot;
    Asser* robot;
    Arduino* arduino;
    tableState* table;

    std::function<int(action*, robotCDFR*, Asser*, Arduino*, tableState*)> runActionPtr;
    std::function<int(tableState*)> validActionPtr;
    std::function<void(tableState*)> goodEndPtr;
    std::function<void(tableState*)> badEndPtr;

    position_t startPostion;
    asser_direction_side startDirection;
    asser_rotation_side startRotation;

    bool noEndPoint = true;
    position_t endPostion;
    asser_direction_side endDirection;
    asser_rotation_side endRotation;

    fsmAction_t currentState = FSMACTION_INIT;
    bool initStat = true;

    std::string actionName;
    bool actionEnable = true;//action autorisée (paramètre général)
    int value;//Valeur fixe de l'action dépend du type d'action

    bool actionDone = false;// Action faîte ou non.
    int actionPoint = 0; // Nombre de points que l'action rapporte à l'équipe en fin de partie

    unsigned long keyMoment;
    bool keyMomentSet = false;
    bool noTetaStart = false;

    obstacle_t obstacle;

    int* zone; 
    int sizeof_action;
public:
    action(std::string name, robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable);
    int runAction(void);
    void setRunAction(std::function<int(action*, robotCDFR*, Asser*, Arduino*, tableState*)> ptr);
    void setStartPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation);
    void setStartPointNoTeta(int x, int y, asser_direction_side Direction, asser_rotation_side rotation);
    void setEndPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation);
    int costAction(void);// renvoie le coût de l'action
    int setActionValue(void);// Fixe la valeur de l'action en points
    void goodEnd(std::function<void(tableState*)> ptr);
    void badEnd(std::function<void(tableState*)> ptr);
    void setCostAction(std::function<int(tableState*)> ptr);
    void resetActionEnable(void);
    void setKeyMoment(unsigned long keyMom);
    bool actionNeedForce(void);

    bool isDone(void);
    void setDone(bool done);

    void setObstacle(obstacle_t* obstacle, int id);

    int getZone(void);
    void action::setZone(int dim);
    obstacle_t* getObstacle(void);
    

    std::string getName(void);
    ~action();

    friend std::ostream& operator<<(std::ostream& os, action& obj) {
        os << obj.getName();
        return os;
    }

private:
    int goToStart(void);
    int goToEnd(void);
};
