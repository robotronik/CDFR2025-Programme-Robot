#pragma once

#include <functional>
#include <string>
#include "navigation.h"
#include "arduinoSubFonction.h"
#include "fonction.h"
#include "tableState.hpp"
#include "constante.h"

class action;

typedef int (*FuncRunPtr)(action*, CmdAsserv*, Arduino*, TableState*);
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
    CmdAsserv* robot;
    Arduino* arduino;
    TableState* table;

    std::function<int(action*, CmdAsserv*, Arduino*, TableState*)> runActionPtr;
    int validActionPtr;
    std::function<void(TableState*,CmdAsserv *)> goodEndPtr;
    std::function<void(TableState*)> badEndPtr;

    position_t startPostion;
    CmdAsserv::Direction startDirection;
    CmdAsserv::Rotation startRotation;

    bool noEndPoint = true;
    position_t endPostion;
    CmdAsserv::Direction endDirection;
    CmdAsserv::Rotation endRotation;

    fsmAction_t currentState = FSM_ACTION_INIT;
    bool initStat = true;

    std::string actionName;
    bool actionEnable = true;

    unsigned long keyMoment;
    bool keyMomentSet = false;
    bool nothetaStart = false;

public:
    action(std::string name, CmdAsserv* irobot, Arduino* iarduino, TableState* itable);
    int runAction(void);
    void setRunAction(std::function<int(action*, CmdAsserv*, Arduino*, TableState*)> ptr);
    void setStartPoint(int x, int y, int theta, CmdAsserv::Direction Direction, CmdAsserv::Rotation rotation);
    void setStartPoint(int x, int y, CmdAsserv::Direction Direction, CmdAsserv::Rotation rotation);
    void setEndPoint(int x, int y, int theta, CmdAsserv::Direction Direction, CmdAsserv::Rotation rotation);
    int costAction(void);
    void goodEnd(std::function<void(TableState*, CmdAsserv*)> ptr);
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
