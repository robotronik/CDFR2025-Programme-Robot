#pragma once

#include <functional>
#include <string>
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "fonction.h"
#include "tableState.hpp"
#include "constante.h"

class action;

typedef int (*FuncRunPtr)(action*, commandesAsservissement*, Arduino*, tableState*);
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
    commandesAsservissement* robot;
    Arduino* arduino;
    tableState* table;

    std::function<int(action*, commandesAsservissement*, Arduino*, tableState*)> runActionPtr;
    int validActionPtr;
    std::function<void(tableState*,commandesAsservissement *)> goodEndPtr;
    std::function<void(tableState*)> badEndPtr;

    position_t startPostion;
    commandesAsservissement::direction startDirection;
    commandesAsservissement::rotation startRotation;

    bool noEndPoint = true;
    position_t endPostion;
    commandesAsservissement::direction endDirection;
    commandesAsservissement::rotation endRotation;

    fsmAction_t currentState = FSMACTION_INIT;
    bool initStat = true;

    std::string actionName;
    bool actionEnable = true;

    unsigned long keyMoment;
    bool keyMomentSet = false;
    bool noTetaStart = false;

public:
    action(std::string name, commandesAsservissement* irobot, Arduino* iarduino, tableState* itable);
    int runAction(void);
    void setRunAction(std::function<int(action*, commandesAsservissement*, Arduino*, tableState*)> ptr);
    void setStartPoint(int x, int y, int teta, commandesAsservissement::direction Direction, commandesAsservissement::rotation rotation);
    void setStartPoint(int x, int y, commandesAsservissement::direction Direction, commandesAsservissement::rotation rotation);
    void setEndPoint(int x, int y, int teta, commandesAsservissement::direction Direction, commandesAsservissement::rotation rotation);
    int costAction(void);
    void goodEnd(std::function<void(tableState*, commandesAsservissement*)> ptr);
    void badEnd(std::function<void(tableState*)> ptr);
    void setCostAction(int num_action, int num_i_action, tableState *itable, int x_start, int y_start);
    void resetActionEnable(void);
    void setKeyMoment(unsigned long keyMom);
    bool actionNeedForce(void);
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
