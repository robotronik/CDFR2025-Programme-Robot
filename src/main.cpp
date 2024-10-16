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
#include "lidarAnalize.h"
#include "lidar.h"
#include "arduino.hpp"
#include "affichage.hpp"
#include "utils.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "restAPI.hpp"

#include "actionContainer.hpp"

// #define DISABLE_LIDAR
// #define TEST_API_ONLY

main_State_t currentState;
TableState tableStatus;
CmdAsserv *robotI2C;
lidarAnalize_t lidarData[SIZEDATALIDAR];
int lidar_count = 0;

Affichage *affichage;
SSD1306 display(0x3C);
Arduino *arduino;

main_State_t nextState;
bool initState;
actionContainer *actionSystem;

std::thread api_server_thread;

// Prototypes
int StartSequence();
void GetLidar();
void EndSequence();

bool isWifiConnected();
void executePythonScript(const std::string &command);

// Signal Management
bool ctrl_c_pressed = false;
void ctrlc(int)
{
    LOG_DEBUG("Stop Signal Recieved");
    ctrl_c_pressed = true;
}
bool ctrl_z_pressed = false;
void ctrlz(int signal)
{
    LOG_DEBUG("Termination Signal Recieved");
    ctrl_z_pressed = true;
}

int main(int argc, char *argv[])
{
    if (StartSequence() != 0)
        return -1;

    // Private counters
    int countStart = 0, countSetHome = 0;
    while (!ctrl_c_pressed)
    {
        LOG_SCOPE("Main");
        sleep(0.01); // We might change this to use a better clock so that the state machine runs at a constant rate

        // Get Sensor Data
        {
            int16_t x, y, teta;
            robotI2C->get_coordinates(x, y, teta);
            tableStatus.robot.pos = {x, y, 0, teta, 0};
            // LOG_GREEN_INFO("X = ", x," Y = ", y, " teta = ", teta);

            if (currentState != FIN)
            {
                GetLidar();
            }
        }

        // State machine
        switch (currentState)
        {
        //****************************************************************
        case INIT:
        {
            if (initState)
            {
                LOG_STATE("INIT");
                int bStateCapteur2 = 0;

                arduino->readCapteur(2, bStateCapteur2);
                if (bStateCapteur2 == 1)
                {
                    robotI2C->set_coordinates(-700, 1100, -90);
                }
                else
                {
                    robotI2C->set_coordinates(-700, -1100, 90); // 90 de base
                }
            }

            int bStateCapteur3, bStateCapteur1;
            arduino->readCapteur(3, bStateCapteur3);
            arduino->readCapteur(1, bStateCapteur1);
            blinkLed(arduino, 2, 500);
            blinkLed(arduino, 1, 500);

            if (bStateCapteur3 == 1 && bStateCapteur1 == 1)
            {
                countSetHome++;
            }
            else
            {
                countSetHome = 0;
            }
            if (countSetHome == 5)
            {
                nextState = INITIALIZE;
                arduino->ledOff(2);
                arduino->ledOff(1);
            }

            break;
        }
        //****************************************************************
        case INITIALIZE:
        {
            if (initState)
            {
                LOG_STATE("INITIALIZE");
                arduino->servoPosition(4, 100);
                robotI2C->enable_motor();
                robotI2C->brake(false);
                arduino->enableStepper(1);
                /*
                TODO : Put it back if needed
                arduino->servoPosition(1,180);
                arduino->servoPosition(2,CLAMPSLEEP);
                arduino->moveStepper(ELEVATORUP,1);
                */
                robotI2C->set_max_speed_forward(MAX_SPEED);
                robotI2C->set_max_speed_backward(MAX_SPEED);
                sleep(1);
            }
            int bStateCapteur2 = 0;
            arduino->readCapteur(2, bStateCapteur2);
            if (bStateCapteur2 == 1)
            {
                tableStatus.robot.colorTeam = YELLOW;
                nextState = SETHOME; // SETHOME pour calibration
                robotI2C->set_coordinates(-700, 1100, -90);
                LOG_INFO("teams : YELLOW");
            }
            else if (bStateCapteur2 == 0)
            {
                tableStatus.robot.colorTeam = BLUE;
                nextState = SETHOME; // SETHOME pour calibration
                robotI2C->set_coordinates(-700, -1100, 90);
                LOG_INFO("teams : BLUE");
            }
            else
            {
                LOG_ERROR("bStateCapteur2 IS NOT THE RIGHT VALUE (0 or 1)");
            }
            break;
        }
        //****************************************************************
        case SETHOME:
        {
            if (initState)
                LOG_STATE("SETHOME");
            if (tableStatus.robot.colorTeam == YELLOW)
            {
                if (initPosition2(&tableStatus, robotI2C, -700, 1280, -180))
                {
                    nextState = WAITSTART;
                }
            }
            else
            {
                if (initPosition2(&tableStatus, robotI2C, -700, -1280, -180))
                {
                    nextState = WAITSTART;
                }
            }

            break;
        }
        case WAITSTART:
        {
            if (initState)
                LOG_STATE("WAITSTART");
            int bStateCapteur1;
            arduino->readCapteur(1, bStateCapteur1);
            if (tableStatus.robot.colorTeam == YELLOW)
            {
                blinkLed(arduino, 1, 500);
            }
            else
            {
                blinkLed(arduino, 2, 500);
            }

            // Counts the number of time the magnet sensor
            if (bStateCapteur1 == 0)
            {
                countStart++;
            }
            else
            {
                countStart = 0;
            }
            if (countStart == 5)
            {
                nextState = RUN;
                arduino->ledOff(1);
                arduino->ledOff(2);
                tableStatus.startTime = millis();
                // actionSystem->initAction(robotI2C, arduino, &(tableStatus));
            }
            break;
        }
        //****************************************************************
        case RUN:
        {
            if (initState)
                LOG_STATE("RUN");
            bool finished = actionSystem->actionContainerRun(robotI2C, &tableStatus);

            if (tableStatus.startTime + 90000 < millis() || tableStatus.FIN || finished)
            {
                nextState = FIN;
            }
            break;
        }

        //****************************************************************
        case FIN:
        {
            if (initState)
            {
                LOG_STATE("FIN");
                affichage->updateScore(tableStatus.getScore());
                arduino->servoPosition(4, 180);
                arduino->servoPosition(1, 180);

                /*
                TODO : Put it back if needed
                arduino->servoPosition(2,CLAMPSTOP);
                */
                arduino->disableStepper(1);
                robotI2C->disable_motor();
                robotI2C->brake(true);
                nextState = STOP;
            }
            break;
        }
        //****************************************************************
        case STOP:
            if (initState)
                LOG_STATE("STOP");
            break;
        //****************************************************************
        default:
            LOG_STATE("default");
            nextState = STOP;
            break;
        }

        initState = false;
        if (currentState != nextState)
        {
            initState = true;
            currentState = nextState;
        }
    }

    EndSequence();
    return 0;
}

int StartSequence()
{
    LOG_INIT();

#ifndef DISABLE_LIDAR
    if (!lidarSetup("/dev/ttyAMA0", 256000))
    {
        LOG_ERROR("cannot find the lidar");
        return -1;
    }

    if (gpioInitialise() < 0)
    {
        LOG_ERROR("cannot initialize lidar gpio speed");
        return -1;
    }
    gpioSetPWMfrequency(18, 20000);
    gpioSetMode(18, PI_OUTPUT);
    gpioSetPWMrange(18, 100);
    gpioPWM(18, 25); // lidar speed
#endif

    signal(SIGTERM, ctrlc);
    signal(SIGINT, ctrlc);
    // signal(SIGTSTP, ctrlz);

    // Start the api server in a separate thread
    api_server_thread = std::thread([&]()
                                    { StartAPIServer(); });

#ifdef TEST_API_ONLY
    TestAPIServer();
    api_server_thread.join();
    return -1;
#endif

    affichage = new Affichage(display);
    affichage->init();

    tableStatus.init(affichage);

    CmdAsserv *robotI2C = new CmdAsserv(I2C_ASSER_ADDR);
    // LOG_SETROBOT(robotI2C);

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

    LOG_INFO("Init sequence done");
    return 0;
}

void GetLidar()
{
    // Averages the position of the opponent over a few scans
    static position_t pos_opponent_avg_sum = {0, 0, 0, 0, 0};
    static int pos_opponent_avg_count = 0, count_pos = 0;

    if (getlidarData(lidarData, lidar_count))
    {
        position_t position = tableStatus.robot.pos;
        position_t pos_opponent = position;
        convertAngularToAxial(lidarData, lidar_count, &position, -100);
        init_position_balise(lidarData, lidar_count, &position);
        convertAngularToAxial(lidarData, lidar_count, &position, 50);
        position_opponent(lidarData, lidar_count, &pos_opponent);

        pos_opponent_avg_sum.x += pos_opponent.x;
        pos_opponent_avg_sum.y += pos_opponent.y;
        pos_opponent_avg_count++;
        if (pos_opponent_avg_count == 5)
        {
            // Calculate the average position
            pos_opponent.x = pos_opponent_avg_sum.x / pos_opponent_avg_count;
            pos_opponent.y = pos_opponent_avg_sum.y / pos_opponent_avg_count;

            double dist;
            // Calculate the distance the opponent moved
            dist = position_distance(tableStatus.pos_opponent, pos_opponent);

            // Save the position to tableStatus
            tableStatus.pos_opponent.x = pos_opponent.x;
            tableStatus.pos_opponent.y = pos_opponent.y;

            // Reset the average counters
            pos_opponent_avg_count = 0;
            pos_opponent_avg_sum.x = 0;
            pos_opponent_avg_sum.y = 0;

            // Execute if opponent has not moved too much
            if (dist < 250)
            {
                opponentInAction(&tableStatus, &pos_opponent);
            }
        }

        if (count_pos == 10)
        {
            affichage->updatePosition(pos_opponent.x, pos_opponent.y);
            count_pos = 0;
        }
        count_pos++;

        int16_t braking_distance;
        robotI2C->get_braking_distance(braking_distance);
        tableStatus.robot.collide = collide(lidarData, lidar_count, braking_distance);
    }
}

void EndSequence()
{

    LOG_DEBUG("Stopping");

    StopAPIServer();
    api_server_thread.join();

    arduino->moveStepper(0, 1);
    gpioPWM(18, 0);
    arduino->servoPosition(4, 180);
    arduino->ledOff(2);
    arduino->ledOff(1);
    arduino->servoPosition(1, 180);

    /*
    TODO : Put it back if needed
    arduino->servoPosition(2,CLAMPSTOP);
    */
    arduino->moveStepper(0, 1);
    robotI2C->disable_motor();
    robotI2C->brake(false);
    robotI2C->stop();
    lidarStop();
    sleep(2);
    arduino->disableStepper(1);
    LOG_DEBUG("PROCESS KILL");
}

bool isWifiConnected()
{
    std::ifstream file("/proc/net/wireless");
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.find("wlan0") != std::string::npos)
            {
                // Si la ligne contient "wlan0", cela indique que l'interface Wi-Fi est présente
                return true;
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier /proc/net/wireless." << std::endl;
    }

    return false;
}

void executePythonScript(const std::string &command)
{
    std::system(command.c_str());
}
