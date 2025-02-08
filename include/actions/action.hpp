#pragma once

#include <functional>
#include <string>
#include "navigation/navigation.h"
#include "actions/functions.h"
#include "defs/tableState.hpp"
#include "defs/constante.h"
#include "i2c/Arduino.hpp"

class action;

// TODO : Remove this typedef
//typedef int (*FuncRunPtr)(action*);
//typedef int (*FuncValidPtr)();


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
    std::function<int(action*)> runActionPtr;
    int validActionPtr;
    std::function<void()> goodEndPtr;
    std::function<void()> badEndPtr;

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
    action(std::string name);
    int runAction();
    void setRunAction(std::function<int(action*)> ptr);
    void setStartPoint(int x, int y, int theta, Direction Direction, Rotation rotation);
    void setStartPoint(int x, int y, Direction Direction, Rotation rotation);
    void setEndPoint(int x, int y, int theta, Direction Direction, Rotation rotation);
    int costAction();
    void goodEnd(std::function<void()> ptr);
    void badEnd(std::function<void()> ptr);
    void setCostAction(int num_action, int num_i_action, int x_start, int y_start);
    void resetActionEnable();
    void setKeyMoment(unsigned long keyMom);
    bool actionNeedForce();
    std::string getName();
    ~action();

    friend std::ostream& operator<<(std::ostream& os, action& obj) {
        os << obj.getName();
        return os;
    }

private:
    nav_return_t goToStart();
    nav_return_t goToEnd();
};
