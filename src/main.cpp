#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <filesystem>
#include <pigpio.h>
#include <thread>
#include <fstream>

#include "main.hpp"
#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "arduino.hpp"
#include "affichage.hpp"
#include "utils.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "restAPI.hpp"

#include "actionContainer.hpp"

//#define DISABLE_LIDAR
//#define TEST_API_ONLY


main_State_t currentState;
TableState tableStatus;
CmdAsserv *robotI2C;
lidarAnalize_t lidarData[SIZEDATALIDAR];

int countStart = 0, count_pos = 0;
int16_t x =0,y=0,teta=0,distance;
int countSetHome = 0;

Affichage *affichage;
SSD1306 display(0x3C);
Arduino* arduino;

main_State_t nextState;
bool initState;
actionContainer* actionSystem;

std::thread api_server_thread;



// Prototypes
int StartSequence();
void GetLidar();
void EndSequence();

bool isWifiConnected();
void executePythonScript(const std::string& command);

// Signal Management
bool ctrl_c_pressed = false;
void ctrlc(int)
{
    LOG_DEBUG("Stop Signal Recieved");
    ctrl_c_pressed = true;
}
bool ctrl_z_pressed = false;
void ctrlz(int signal) {
    LOG_DEBUG("Termination Signal Recieved");
    ctrl_z_pressed = true;
}


int main(int argc, char *argv[]) {
    if (StartSequence() != 0)
        return -1;

    while (!ctrl_c_pressed) {

        LOG_SCOPE("Main");
        sleep(0.01);
        
        if(currentState != FIN){
            GetLidar();
        }
        switch (currentState) {
            //****************************************************************
            case INIT:{
                if(initState){ 
                    LOG_STATE("INIT");
                    int bStateCapteur2 = 0;
                    
                    arduino->readCapteur(2,bStateCapteur2);
                    if(bStateCapteur2 == 1){
                        robotI2C->set_coordinates(-700,1100,- 90);
                    }
                    else{
                        robotI2C->set_coordinates(-700,-1100,90); //90 de base
                    }
                }
    
                int bStateCapteur3 = 0;
                int bStateCapteur1 = 0;
                arduino->readCapteur(3,bStateCapteur3);
                arduino->readCapteur(1,bStateCapteur1);
                blinkLed(arduino,2,500);
                blinkLed(arduino,1,500);
                
                if(bStateCapteur3 == 1 && bStateCapteur1 == 1){
                    countSetHome ++;
                }
                else{
                    countSetHome = 0;
                }
                if(countSetHome == 5){
                    nextState = INITIALIZE;
                    arduino->ledOff(2);
                    arduino->ledOff(1);
                }
                
                
                break;
            }
            //****************************************************************
            case INITIALIZE:{
                if(initState){
                    LOG_STATE("INITIALIZE");
                    arduino->servoPosition(4,100);
                    robotI2C->enable_motor();
                    robotI2C->brake(false);
                    arduino->enableStepper(1);
                    arduino->servoPosition(1,180);
                    arduino->servoPosition(2,CLAMPSLEEP);
                    arduino->moveStepper(ELEVATORUP,1);
                    robotI2C->set_max_speed_forward(MAX_SPEED);
                    robotI2C->set_max_speed_backward(MAX_SPEED);
                    sleep(1);
                }
                int bStateCapteur2 = 0;
                arduino->readCapteur(2,bStateCapteur2);
                if(bStateCapteur2 == 1){
                    tableStatus.robot.colorTeam = YELLOW;
                    nextState = SETHOME; //SETHOME pour calibration
                    robotI2C->set_coordinates(-700,1100,-90);
                    LOG_INFO("teams : YELLOW");
                }
                else if(bStateCapteur2 == 0){
                    tableStatus.robot.colorTeam = BLUE;
                    nextState = SETHOME; //SETHOME pour calibration
                    robotI2C->set_coordinates(-700,-1100,90);
                    LOG_INFO("teams : BLUE");
                }
                else{
                    LOG_ERROR("bStateCapteur2 IS NOT THE RIGHT VALUE (0 or 1)");
                }
                break;
            }
            //****************************************************************
            case SETHOME:{
                if (initState) 
                    LOG_STATE("SETHOME");
                if(tableStatus.robot.colorTeam == YELLOW){
                    if(initPosition2(&tableStatus,robotI2C,-700,1280,-180)){
                        nextState = WAITSTART;
                    }
                }
                else{
                    if(initPosition2(&tableStatus,robotI2C,-700,-1280,-180)){
                        nextState = WAITSTART;
                    }
                }
                
                break;
            }            
            case WAITSTART:{
                if(initState)
                    LOG_STATE("WAITSTART");
                int bStateCapteur1 = 0;
                arduino->readCapteur(1,bStateCapteur1);
                if(tableStatus.robot.colorTeam == YELLOW){
                    blinkLed(arduino,1,500);
                }
                else{
                    blinkLed(arduino,2,500);
                }
                
               if(bStateCapteur1 == 0){
                    countStart ++;
                }
                else{
                    countStart = 0;
                }
                if(countStart == 5){
                    nextState = RUN;
                    arduino->ledOff(1);
                    arduino->ledOff(2);
                    tableStatus.startTime = millis();
                    tableStatus.robot.robotHavePlante = false;
                    actionSystem->initAction( robotI2C, arduino, &(tableStatus));
                    LOG_GREEN_INFO("RE INITIALIZE PLANTESTOCK AND JARDINIERE");
                    for(int i = 0; i<6;i++){tableStatus.planteStockFull[i].etat = true;} //mettre true normalement
                    for(int i = 0; i<6;i++){tableStatus.JardiniereFull[i].etat = false;}
                    for(int i = 0; i<9;i++){tableStatus.panneauSolaireRotate[i].etat = false;}
                }
                break;
            }
            //****************************************************************
            case RUN:{
                if(initState) 
                    LOG_STATE("RUN");
                bool finish;
                if(tableStatus.robot.colorTeam == YELLOW){
                    finish = actionSystem->actionContainerRun(robotI2C,&tableStatus);
                    //finish =  FSMMatch(mainRobot,robotI2C, arduino);
                }
                else{
                    finish = actionSystem->actionContainerRun(robotI2C,&tableStatus);
                    //finish =  TestPinceFSM(mainRobot,robotI2C, arduino);
                    //finish =  FSMMatch(mainRobot,robotI2C, arduino);
                }
                if(tableStatus.startTime+90000 < millis()){
                    nextState = FIN;
                }
                if (tableStatus.FIN){
                    nextState = FIN;
                }
                if(finish){
                    nextState = FIN;
                }
                break;
            }
        
            //****************************************************************
            case FIN:
                if(initState){
                    LOG_STATE("FIN");
                    affichage->updateScore(tableStatus.getScore());
                    arduino->servoPosition(4,180);
                    arduino->servoPosition(1,180);
                    arduino->servoPosition(2,CLAMPSTOP);
                    arduino->disableStepper(1);
                    robotI2C->disable_motor();
                    robotI2C->brake(true);
                    nextState = STOP;
                } 
                break;
            //****************************************************************
            case STOP:
                if(initState) 
                    LOG_STATE("STOP");
                break;
            //****************************************************************
            default:
                LOG_STATE("default");
                nextState = STOP;
                break;
        }

        initState = false;
        if(currentState != nextState){
            initState = true;
        }
        currentState = nextState;
    }

    EndSequence();
    return 0;
}

int StartSequence(){
    LOG_INIT();

#ifndef DISABLE_LIDAR
    if(!lidarSetup("/dev/ttyAMA0",256000)){
        LOG_ERROR("cannot find the lidar");
        return -1;
    }

    if (gpioInitialise() < 0) {
        LOG_ERROR("cannot initialize lidar gpio speed");
        return -1;
    }
    gpioSetPWMfrequency(18, 20000);
    gpioSetMode(18, PI_OUTPUT);
    gpioSetPWMrange(18, 100);
    gpioPWM(18, 25);//lidar speed
#endif

    signal(SIGTERM, ctrlc);
    signal(SIGINT, ctrlc);
    //signal(SIGTSTP, ctrlz);

    // Start the api server in a separate thread
    api_server_thread = std::thread([&]() {
        StartAPIServer();
    });

#ifdef TEST_API_ONLY
    while(!ctrl_c_pressed){
        sleep(1);
    }
    StopAPIServer();
    api_server_thread.join();
    return -1;
#endif

    affichage = new Affichage(display);
    affichage->init();

    tableStatus.init(affichage);
    
    robotI2C = new CmdAsserv(I2C_ASSER_ADDR);
    //LOG_SETROBOT(robotI2C);

    arduino = new Arduino(I2C_ARDUINO_ADDR);

    currentState = INIT;
    nextState = INIT;
    initState = true;

    actionSystem = new actionContainer(robotI2C, arduino, &tableStatus);


    // arduino->enableStepper(1);
    // arduino->servoPosition(1,180);
    // arduino->servoPosition(2,0);
    // arduino->moveStepper(2200,1);
    // while(!ctrl_c_pressed);
    // ctrl_c_pressed = false;
    // while(!catchPlant(arduino));
    // while(!ctrl_c_pressed);
    // ctrl_c_pressed = false;
    // while(!releasePlant(arduino));
    // while(!ctrl_c_pressed);

    // std::string colorTest = tableStatus.colorTeam == YELLOW ? "YELLOW" : "BLUE";
    // std::filesystem::path exe_pathTest = std::filesystem::canonical(std::filesystem::path(argv[0])).parent_path();
    // std::filesystem::path python_script_pathTest = exe_pathTest / "../startPAMI.py";
    // std::string commandTest = "python3 " + python_script_pathTest.string() + " " +  colorTest;
    // std::thread python_threadTest(executePythonScript,commandTest);
    return 0;
}

void GetLidar(){
    // LIDAR could be threadded
    int count = SIZEDATALIDAR;
    if(getlidarData(lidarData,count)){
        robotI2C->get_coordinates(x,y,teta);
        position_t position = {x,y,0,teta,0};
        position_t pos_ennemie = {x,y,0,teta,0};
        convertAngularToAxial(lidarData,count,&position,-100);
        init_position_balise(lidarData,count, &position);
        //LOG_GREEN_INFO("X = ", position.x,"Y = ", position.y, "teta = ", position.teta);
        convertAngularToAxial(lidarData,count,&position,50);
        position_ennemie(lidarData, count, &pos_ennemie);
        
        tableStatus.ennemie.x += pos_ennemie.x;
        tableStatus.ennemie.y += pos_ennemie.y;
        tableStatus.nb += 1;
        if (tableStatus.nb == 5){
            pos_ennemie.x = tableStatus.ennemie.x/tableStatus.nb;
            pos_ennemie.y = tableStatus.ennemie.y/tableStatus.nb;
            distance = sqrt(pow(tableStatus.prev_pos.x - pos_ennemie.x,2)+ pow(tableStatus.prev_pos.y - pos_ennemie.y,2));
            tableStatus.prev_pos.x = pos_ennemie.x;
            tableStatus.prev_pos.y = pos_ennemie.y;
            tableStatus.nb = 0; tableStatus.ennemie.x = 0; tableStatus.ennemie.y = 0;
            if( distance < 250) {ennemieInAction(&tableStatus, &pos_ennemie);}
        }
        
        
        
        if (count_pos == 10){
            affichage->updatePosition(pos_ennemie.x,pos_ennemie.y);
            count_pos = 0;
        }
        count_pos ++;
        if(ctrl_z_pressed){
            ctrl_z_pressed = false;
            pixelArtPrint(lidarData,count,50,50,100,position);
        }                
        robotI2C->get_braking_distance(distance);
        tableStatus.robot.collide = collide(lidarData,count,distance);
    }
}

void EndSequence(){

    LOG_DEBUG("Stopping");

    StopAPIServer();
    api_server_thread.join();

    arduino->moveStepper(0,1);
    gpioPWM(18, 0);
    arduino->servoPosition(4,180);
    arduino->ledOff(2);
    arduino->ledOff(1);
    arduino->servoPosition(1,180);
    arduino->servoPosition(2,CLAMPSTOP);
    arduino->moveStepper(0,1);
    robotI2C->disable_motor();
    robotI2C->brake(false);
    robotI2C->stop();
    lidarStop();
    sleep(2);
    arduino->disableStepper(1);
    LOG_DEBUG("PROCESS KILL");
}






bool isWifiConnected() {
    std::ifstream file("/proc/net/wireless");
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.find("wlan0") != std::string::npos) {
                // Si la ligne contient "wlan0", cela indique que l'interface Wi-Fi est présente
                return true;
            }
        }
        file.close();
    } else {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier /proc/net/wireless." << std::endl;
    }

    return false;
}

void executePythonScript(const std::string& command) {
    std::system(command.c_str());
}
