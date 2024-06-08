#pragma once

#include <unistd.h>
#include <rplidar.h>
#include "config.h"

#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))

using namespace sl;


static inline void delay(sl_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}


bool lidarSetup(const char* serialPort ,int baudrate);

bool getlidarData(lidarAnalize_t* data, int& count);

void lidarStop(void);