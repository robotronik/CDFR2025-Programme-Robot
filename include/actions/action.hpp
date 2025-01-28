#pragma once

#include <functional>
#include <string>
#include "navigation/navigation.h"
#include "actions/functions.h"
#include "defs/tableState.hpp"
#include "defs/constante.h"
#include "i2c/arduino.hpp"

class action;

typedef int (*FuncRunPtr)(action*, Asserv*, Arduino*, TableState*);
typedef int (*FuncValidPtr)(TableState*);


class action
{
private: 


    typedef enum{
        FSM_ACTION_INIT,
        FSM_ACTION_MOVESTART,
        FSM_ACTION_ACTION,
        FSM_ACTION_MOVEEND
    }fsmAction_t;

private:
    Asserv* robot;
    Arduino* arduino;
    TableState* table;

    std::function<int(action*, Asserv*, Arduino*, TableState*)> runActionPtr;
    int validActionPtr;
    std::function<void(TableState*,Asserv *)> goodEndPtr;
    std::function<void(TableState*)> badEndPtr;

    position_t startPostion;
    Direction startDirection;
    Rotation startRotation;

    bool noEndPoint = true;
    position_t endPostion;
    Direction endDirection;
    Rotation endRotation;

    fsmAction_t currentState = FSM_ACTION_INIT;
    bool initStat = true;

    std::string actionName;
    bool actionEnable = true;

    unsigned long keyMoment;
    bool keyMomentSet = false;
    bool nothetaStart = false;

public:
    action(std::string name, Asserv* irobot, Arduino* iarduino, TableState* itable);
    int runAction(void);
    void setRunAction(std::function<int(action*, Asserv*, Arduino*, TableState*)> ptr);
    void setStartPoint(int x, int y, int theta, Direction Direction, Rotation rotation);
    void setStartPoint(int x, int y, Direction Direction, Rotation rotation);
    void setEndPoint(int x, int y, int theta, Direction Direction, Rotation rotation);
    int costAction(void);
    void goodEnd(std::function<void(TableState*, Asserv*)> ptr);
    void badEnd(std::function<void(TableState*)> ptr);
    void setCostAction(int num_action, int num_i_action, TableState *itable, int x_start, int y_start);
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
    nav_return_t goToStart(void);
    nav_return_t goToEnd(void);
};
