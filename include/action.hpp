#pragma once

#include <functional>
#include <string>
#include "deplacement.h"
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
        FSMACTION_INIT,
        FSMACTION_MOVESTART,
        FSMACTION_ACTION,
        FSMACTION_MOVEEND
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
    CmdAsserv::direction startDirection;
    CmdAsserv::rotation startRotation;

    bool noEndPoint = true;
    position_t endPostion;
    CmdAsserv::direction endDirection;
    CmdAsserv::rotation endRotation;

    fsmAction_t currentState = FSMACTION_INIT;
    bool initStat = true;

    std::string actionName;
    bool actionEnable = true;

    unsigned long keyMoment;
    bool keyMomentSet = false;
    bool noTetaStart = false;

public:
    action(std::string name, CmdAsserv* irobot, Arduino* iarduino, TableState* itable);
    int runAction(void);
    void setRunAction(std::function<int(action*, CmdAsserv*, Arduino*, TableState*)> ptr);
    void setStartPoint(int x, int y, int teta, CmdAsserv::direction Direction, CmdAsserv::rotation rotation);
    void setStartPoint(int x, int y, CmdAsserv::direction Direction, CmdAsserv::rotation rotation);
    void setEndPoint(int x, int y, int teta, CmdAsserv::direction Direction, CmdAsserv::rotation rotation);
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
    int goToStart(void);
    int goToEnd(void);
};
