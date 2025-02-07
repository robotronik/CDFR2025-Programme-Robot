#pragma once

#include <unistd.h>
#include <rplidar.h>
#include "defs/structs.hpp"
#include "defs/constante.h"

#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))

using namespace sl;

class Lidar {
public:
    Lidar();
    ~Lidar();

    lidarAnalize_t data[SIZEDATALIDAR];
    int count;

    bool setup(const char* serialPort, int baudrate);

    bool getData();

    void startSpin();
    void stopSpin();

    void Stop(void);
    void Delete();
private:
    bool isSpinning;
    ILidarDriver* drv;
};

static inline void delay(sl_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}