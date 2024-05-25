#pragma once

#include "tableState.hpp"

class robotCDFR
{
private:

public:
    tableState tableStatus;
    int collide;
    /* data */
public:
    robotCDFR(/* args */);
    void robotCDFRloop(void);
    ~robotCDFR();
};


