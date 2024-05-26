#include "lidar.h"
#include "lidarAnalize.h"
#include "logger.hpp"
#include <pigpio.h>
#define SIZEDATALIDAR 10000

int main(int argc, char *argv[]) {
    LOG_INIT();


    if(!lidarSetup("/dev/ttyAMA0",256000)){
        LOG_ERROR("cannot find the lidar");
        return -1;
    }

#ifndef DISABLE_LIDAR
    if (gpioInitialise() < 0) {
        LOG_ERROR("cannot initialize lidar gpio speed");
        return 1;
    }
    gpioSetPWMfrequency(18, 25000);
    gpioSetMode(18, PI_OUTPUT);
    gpioSetPWMrange(18, 100);
    gpioPWM(18, 50);//lidar speed
#endif

lidarAnalize_t lidarData[SIZEDATALIDAR];  

while (1) {

        LOG_SCOPE("Main");
        sleep(0.01);
        
       
        int count = SIZEDATALIDAR;
            if(getlidarData(lidarData,count)){
                int x, y, teta;
                int distance;
                position_t position = {x,y,teta,0};
                convertAngularToAxial(lidarData,count,position);
                if(1){
                //if(crtl_z_pressed){
                    //ctrl_z_pressed = false;
                    pixelArtPrint(lidarData,count,50,50,100,position);
                }                
                //printAngular(lidarData,count);
                
                }
            }
}
       