#include "arduinoSubFonction.h"

int pullpush(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        LOG_INFO("push pull");
        arduino->servoPosition(1,0);
        step++;
        startTime = millis()+600;
    }
    else if(step == 1 && startTime < millis()){
        arduino->servoPosition(1,180);
        step++;
    }
    else if(step == 2 ){
        step = 0;
        bret = true;
    }
    return bret;
}

int catchPlant(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        LOG_INFO("catch plant");
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
        
    }
    else if(step == 1){
        arduino->moveStepper(ELEVATORPLANT,1);
        step++;
        startTime = millis()+1800;
    }
    else if(step == 2 && startTime < millis()){
        arduino->servoPosition(2,CLAMPCLOSE);
        step++;
        startTime = millis()+DELAYOPENCLOSE;
    }
    else if(step == 3 && startTime < millis()){
        arduino->moveStepper(ELEVATORJARDINIERE,1);
        step++;
        startTime = millis()+200;
    }
    else if(step == 4 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;
}
int catchPlant2(Arduino* arduino){ //catch plant in jardininiere
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        LOG_INFO("catch plant");
        arduino->servoPosition(2,33);
        step++;
        
    }
    else if(step == 1){
        arduino->moveStepper(1400,1);
        step++;
        startTime = millis()+500;
    }
    else if(step == 2 && startTime < millis()){
        arduino->servoPosition(2,CLAMPCLOSE);
        step++;
        startTime = millis()+DELAYOPENCLOSE;
    }
    else if(step == 3 && startTime < millis()){
        arduino->moveStepper(ELEVATORJARDINIERE,1);
        step++;
        startTime = millis()+1200;
    }
    else if(step == 4 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;
}

int releasePlant(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        LOG_INFO("release plant");
        arduino->moveStepper(2200,1);
        step++;
        startTime = millis()+1400;
    }
    else if(step == 1 && startTime < millis()){
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
    }
    else if(step == 2){
        arduino->moveStepper(ELEVATORUP,1);
        step++;
        startTime = millis()+1300;
    }
    else if(step == 3 && startTime < millis()){
        arduino->servoPosition(2,CLAMPSLEEP);
        step++;
        startTime = millis()+200;
    }
    else if(step == 4 && startTime < millis()){
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
        startTime = millis()+200;
    }
    else if(step == 5 && startTime < millis()){
        arduino->servoPosition(2,CLAMPSLEEP);
        step++;
        
    }
    else if(step == 6){
        step = 0;
        bret = true;
    }
    return bret;
}

int FastReleasePlant(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        LOG_INFO("release plant");
        arduino->moveStepper(1600,1);
        step++;
        startTime = millis()+300;
    }
    else if(step == 1 && startTime < millis()){
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
    }
    else if(step == 2){
        arduino->moveStepper(3000,1);
        step++;
        startTime = millis()+1000;
    }
    else if(step == 3 && startTime < millis()){
        arduino->servoPosition(2,CLAMPSLEEP);
        step++;
        startTime = millis()+100;
    }
    else if(step == 4 && startTime < millis()){
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
        startTime = millis()+100;
    }
    else if(step == 5 && startTime < millis()){
        arduino->servoPosition(2,CLAMPSLEEP);
        step++;
        startTime = millis()+100;
    }
    else if(step == 6 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;
}

void blinkLed(Arduino* arduino,int LedNb,int periode){
    static unsigned long startTime = millis();
    static int step = 0;

    if(step == 0 && startTime < millis()){
        arduino->ledOn(LedNb);
        step++;
        startTime = millis()+periode/2;
    }
    else if(step == 1 && startTime < millis()){
        arduino->ledOff(LedNb);
        step = 0;
        startTime = millis()+periode/2;
    }
    return;
}