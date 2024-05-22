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
    int x = 0, y= 0, teta= 0;
    
    printf("\ntest\n");
    if(!lidarSetup("/dev/ttyUSB0",256000)){
        LOG_ERROR("cannot find the lidar");
    
    }
    
    sleep(4);
    getlidarData(lidarData,count);
    position_t position = {x,y,teta,0};
    convertAngularToAxial(lidarData,count,position);
    //printAngular(lidarData, count);
    pixelArtPrint(lidarData, count, 50, 50, position);
    

    lidarStop();
    }