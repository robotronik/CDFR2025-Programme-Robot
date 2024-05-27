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

    
    printf("\ntest\n");
    if(!lidarSetup("/dev/ttyUSB1",256000)){
        LOG_ERROR("cannot find the lidar");
    }
    position_t position = {x,y,teta,dist,0};
    sleep(4);
    //convertAngularToAxial(lidarData,count,position); //position ennemi
    //pixelArtPrint(lidarData, count, 50, 50, 100, position);

    //update position
    for (int i = 0; i < 1; i++){
    //while (1) {
        getlidarData(lidarData,count);
        init_position(lidarData, count, &position);
        printf("\n\n  xM = %f / yM = %f / teta = %f / dist = %f",position.x,position.y,position.teta, position.dist);
        position.x = 0;
        position.y = 0;
        position.teta = 0;
        position.dist = 0;
        delay(500);
    }
        lidarStop();
    
    }