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
    int x, y, teta;
    int distance;

    printf("\ntest\n");
    if(!lidarSetup("/dev/ttyUSB0",256000)){
        LOG_ERROR("cannot find the lidar");
    
    }
    

    getlidarData(lidarData,count);
    position_t position = {x,y,teta,0};
    convertAngularToAxial(lidarData,count,position);
    pixelArtPrint(lidarData, count, 50, 50, position);
    /*
    for (int i = 0; i < count; i++){
        printf("dist = %f , angle = %f \n", lidarData[i].dist, lidarData[i].angle);
    }*/

    lidarStop();
    }