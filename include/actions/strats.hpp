#pragma once
#include "main.hpp"
#include "utils/logger.hpp"
#include "actions/revolver.hpp"

inline void check(colorTeam_t color, int strategy){
    // Check if the color and strategy are valid
    if (color == NONE || strategy < 1 || strategy > 4)
        LOG_ERROR("Invalid color (", color, ") or strategy (", strategy, ")");
}

// Function to handle the strategy
inline position_t StratStartingPos(){
    // Returns the starting position of the robot
    colorTeam_t color = tableStatus.robot.colorTeam;
    int strategy = tableStatus.strategy;
    check(color, strategy);
    position_t pos = {0, 0, 0};

    // First set the position as blue
    switch (strategy)
    {
    case 1:
        pos = {73, -1500+225, 90}; break;
    case 2:
        pos = {200, -1500+140, -90}; break;
    case 3:
        pos = {0, 0, 0}; break;
    case 4:
        pos = {0, 0, 0}; break;
    }
    if (color == YELLOW)
        position_robot_flip(pos);
    return pos;
}
inline bool StratGather(int& stockNum, int& stockOffset){
    // Returns true if the robot can gather a stock
    colorTeam_t color = tableStatus.robot.colorTeam;
    int strategy = tableStatus.strategy;
    check(color, strategy);

    // If barrel is full, return false
    if (isRevolverFull())
        return false;

    // Depending on the strategy, it looks for the first available stock in a list of stocks
    // and returns the stock number and the offset
    int todo_stocks[9];
    int num;
    // TODO add a maximum timer after wich the robot should stop looking for a stock
    switch (strategy)
    {
    case 1:
        todo_stocks[0] = 9;
        todo_stocks[1] = 4;
        todo_stocks[2] = 2;
        todo_stocks[3] = 3;
        num = 4;
        break;
    case 2:
        num = 0;
        break;
    case 3:
        num = 0;
        break;
    case 4:
        num = 0;
        break;
    }
    if (color == YELLOW){
        // Invert the stocks
        for (int i = 0; i < num; i++)
            todo_stocks[i] = (todo_stocks[i] + STOCK_COUNT/2) % STOCK_COUNT;
    }
    int i = 0;
    while (i < num){
        if (tableStatus.avail_stocks[todo_stocks[i]]){
            stockNum = todo_stocks[i];
            stockOffset = getBestStockPositionOff(stockNum, tableStatus.robot.pos);
            return true;
        }
        i++;
    }
    LOG_GREEN_INFO("No stock available");
    return false;
}
inline void StratConstruct(int& zone){
    // Returns the zone to build
    colorTeam_t color = tableStatus.robot.colorTeam;
    int strategy = tableStatus.strategy;
    check(color, strategy);

    // Depending on the strategy, it returns the zone to build
    // TODO

    zone = 0;
}
// bool StratDeployBanner();
// bool StratNavHome();

inline void StratTimes(unsigned long& maxGather, unsigned long& maxBuild){
    // Returns the maximum time to gather and build
    // Returns the time in ms
    // Remember, a match lasts 100s;
    colorTeam_t color = tableStatus.robot.colorTeam;
    int strategy = tableStatus.strategy;
    check(color, strategy);
    maxGather = 0;
    maxBuild = 0;
    switch (strategy)
    {
    case 1:
        maxGather = 50;
        maxBuild = 90;
        break;
    case 2:
        maxGather = 50;
        maxBuild = 90;
        break;
    case 3:
        maxGather = 50;
        maxBuild = 90;
        break;
    case 4: 
        maxGather = 50;
        maxBuild = 90;
        break;
    }
    maxGather *= 1000;
    maxBuild *= 1000;
}