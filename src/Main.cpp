#include "lidar.h"
#include "lidarAnalize.h"
#include "logger.hpp"
#include <pigpio.h>
#include <unistd.h>
#define SIZEDATALIDAR 10000

//lidarAnalise_t : valid, angle, dist, x, y, onTable

lidarAnalize_t lidarData[SIZEDATALIDAR];  
int count = SIZEDATALIDAR;

int main() {
    double x = 0, y= 0, teta= 0, dist = 10000;

    
    if(!lidarSetup("/dev/ttyUSB1",256000)){
        LOG_ERROR("cannot find the lidar");
    }
    position_t position = {x,y,teta,dist,0};
    sleep(4);
    for (int i = 0; i < 1; i++){
        getlidarData(lidarData,count);
        convertAngularToAxial(lidarData, count, &position);
        erreur(lidarData,count,x,y);
        delay(500);
    }

    lidarStop();
    
    }