#pragma once
#include "main.hpp"
#include "utils/logger.hpp"

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
    if (color == YELLOW){
        pos.y = -pos.y;
        pos.theta = -pos.theta;
    }
    return pos;
}
inline bool StratGather(int& stockNum, int& stockOffset){
    // Returns true if the robot can gather a stock
    colorTeam_t color = tableStatus.robot.colorTeam;
    int strategy = tableStatus.strategy;
    check(color, strategy);

    // Depending on the strategy, it looks for the first available stock in a list of stocks
    // and returns the stock number and the offset
    int stocks[9];
    int num;
    // TODO add a maximum timer after wich the robot should stop looking for a stock
    switch (strategy)
    {
    case 1:
        stocks[0] = 9;
        stocks[1] = 4;
        stocks[2] = 2;
        stocks[3] = 3;
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
            stocks[i] = (stocks[i] + STOCK_COUNT/2) % STOCK_COUNT;
    }
    int i = 0;
    while (i < num){
        if (tableStatus.avail_stocks[stocks[i]]){
            stockNum = stocks[i];
            stockOffset = getBestStockPositionOff(stockNum, tableStatus.robot.pos);
            return true;
        }
        i++;
    }
    return false;
}
inline void StratBuild(int& zone){
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
