#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <filesystem>
#include <thread>
#include <fstream>
#include <unistd.h>  // for usleep

#include "main.hpp"
#include "lidarAnalize.h"
#include "lidar.h"
#include "utils.h"
#include "logger.hpp"
#include "restAPI.hpp"
#include "gpio.h"

#include "actionContainer.hpp"

// #define DISABLE_LIDAR
// #define TEST_API_ONLY
// #define DISABLE_LIDAR_BEACONS

main_State_t currentState;
TableState tableStatus;
CmdAsserv *robotI2C;
lidarAnalize_t lidarData[SIZEDATALIDAR];
int lidar_count = 0;

Arduino *arduino;

main_State_t nextState;
bool initState;
actionContainer *actionSystem;
bool manual_ctrl;

std::thread api_server_thread;

// Prototypes
int StartSequence();
void GetLidarV2();
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
    unsigned long loopStartTime;
    while (!ctrl_c_pressed)
    {
        LOG_SCOPE("Main");
        loopStartTime = _millis();

        // Get Sensor Data
        {
            int16_t x, y, theta;
            robotI2C->get_coordinates(x, y, theta);
            tableStatus.robot.pos = {x, y, theta};
            // LOG_GREEN_INFO("X = ", x," Y = ", y, " theta = ", theta);

            if (currentState != FIN)
            {
#ifndef DISABLE_LIDAR
                GetLidarV2();
                navigationOpponentDetection();
#endif
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
                    robotI2C->set_coordinates(-770, 1390, -90);
                }
                else
                {
                    robotI2C->set_coordinates(-770, -1390, 90); // 90 de base
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
                arduino->enableStepper(1);
                robotI2C->set_motor_state(true);
                robotI2C->set_brake_state(false);
                robotI2C->set_linear_max_speed(MAX_SPEED);
                while(robotI2C->get_command_buffer_size() != 0); //wait end of all action above
            }
            int bStateCapteur2 = 0;
            arduino->readCapteur(2, bStateCapteur2);
            if (bStateCapteur2 == 1)
            {
                tableStatus.robot.colorTeam = YELLOW;
                nextState = WAITSTART; // SETHOME pour calibration
                robotI2C->set_coordinates(-770, 1390, -90);
                LOG_INFO("teams : YELLOW");
            }
            else if (bStateCapteur2 == 0)
            {
                tableStatus.robot.colorTeam = BLUE;
                nextState = WAITSTART; // SETHOME pour calibration
                robotI2C->set_coordinates(-770, -1390, 90);
                LOG_INFO("teams : BLUE");
            }
            else
            {
                LOG_ERROR("bStateCapteur2 IS NOT THE RIGHT VALUE (0 or 1)");
            }
            if (manual_ctrl)
                nextState = MANUAL;
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
                countStart++;
            else
                countStart = 0;

            if (countStart == 5)
            {
                nextState = RUN;
                arduino->ledOff(1);
                arduino->ledOff(2);
                tableStatus.startTime = _millis();
                // actionSystem->initAction(robotI2C, arduino, &(tableStatus));
            }
            if (manual_ctrl)
                nextState = MANUAL;
            break;
        }
        //****************************************************************
        case RUN:
        {
            if (initState){
                LOG_STATE("RUN");
                tableStatus.startTime = _millis();
                actionSystem->initAction(robotI2C, arduino, &(tableStatus));
            }
            bool finished = actionSystem->actionContainerRun(robotI2C, &tableStatus);

            if (tableStatus.startTime + 90000 < _millis() || tableStatus.FIN || finished)
            {
                nextState = FIN;
            }
            break;
        }


        //****************************************************************
        case MANUAL:
        {
            if (initState)
                LOG_STATE("MANUAL");

            if (!manual_ctrl)
                nextState = FIN;
            break;
        }

        //****************************************************************
        case FIN:
        {
            if (initState)
            {
                LOG_STATE("FIN");
                arduino->servoPosition(4, 180);
                arduino->servoPosition(1, 180);

                /*
                TODO : Put it back if needed
                arduino->servoPosition(2,CLAMPSTOP);
                */
                arduino->disableStepper(1);
                robotI2C->set_motor_state(false);
                robotI2C->set_brake_state(false);
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

        // Check if state machine is running above loop time
        if (_millis() > loopStartTime + LOOP_TIME_MS){
            LOG_WARNING("Loop took more than " , LOOP_TIME_MS, "ms to execute (", (_millis() - loopStartTime), " ms)");
        }
        //State machine runs at a constant rate
        while(_millis() < loopStartTime + LOOP_TIME_MS){    
            usleep(100);  // sleep for 100 microseconds
        }
    }

    EndSequence();
    return 0;
}

int StartSequence()
{
    LOG_INIT();

    signal(SIGTERM, ctrlc);
    signal(SIGINT, ctrlc);
    // signal(SIGTSTP, ctrlz);

#ifndef DISABLE_LIDAR
    if (!lidarSetup("/dev/ttyAMA0", 256000))
    {
        LOG_ERROR("cannot find the lidar");
        return -1;
    }

    // Setup the PWM on pin 18
    if (GPIO_SetupPWMMotor() == -1) {
        fprintf(stderr, "Unable to initialize gpio\n");
        return -1;
    }
#endif


    // Start the api server in a separate thread
    api_server_thread = std::thread([&]()
                                    { StartAPIServer(); });

#ifdef TEST_API_ONLY
    tableStatus.init(affichage);
    TestAPIServer();
    // Wait for program termination
    while(!ctrl_c_pressed){
        sleep(0.1);
#ifndef DISABLE_LIDAR
        getlidarData(lidarData, lidar_count);
#endif
    }
    StopAPIServer();
    api_server_thread.join();
    return -1;
#endif

    tableStatus.init();

    robotI2C = new CmdAsserv(I2C_ASSER_ADDR);
    // LOG_SETROBOT(robotI2C);

    initNavigation(robotI2C, &tableStatus);

    arduino = new Arduino(I2C_ARDUINO_ADDR);

    currentState = INIT;
    nextState = INIT;
    initState = true;
    manual_ctrl = false;

    actionSystem = new actionContainer(robotI2C, arduino, &tableStatus);

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
    static position_t pos_opponent_avg_sum = {0, 0, 0};
    static int pos_opponent_avg_count = 0, count_pos = 0;

    if (getlidarData(lidarData, lidar_count))
    {
        position_t position = tableStatus.robot.pos;
        position_t pos_opponent = position;
        convertAngularToAxial(lidarData, lidar_count, &position, -100);
        //init_position_balise(lidarData, lidar_count, &position);
        convertAngularToAxial(lidarData, lidar_count, &position, 50);
        if (position_opponent(lidarData, lidar_count, position, &pos_opponent)){
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
        }


        if (count_pos == 10)
            count_pos = 0;
        count_pos++;

        int16_t braking_distance = robotI2C->get_braking_distance();
        tableStatus.robot.collide = collide(lidarData, lidar_count, braking_distance);
    }
}
void GetLidarV2()
{
    // Implements a low passfilter
    // Simple exponential moving average (EMA)
    
    // Smoothing factor (0 < alpha < 1)
    const float alpha = 0.5f; // Adjust this value for more or less smoothing on the opponent robot posititon

    static position_t pos_opponent_filtered = {0, 0, 0};
    static bool first_reading = true;

    if (getlidarData(lidarData, lidar_count))
    {
        position_t position;
        colorTeam_t color;
        // TODO : Add offset to lidar robot pos
#ifndef DISABLE_LIDAR_BEACONS
        if (position_robot_beacons(lidarData, lidar_count, &position, tableStatus.robot.colorTeam, &color)){
            LOG_GREEN_INFO("Successfully found the robot's position using beacons");
            LOG_GREEN_INFO("X = ", position.x," Y = ", position.y, " theta = ", position.theta);
            // TODO, apply that new position to the tableStatus robot pos and the asserv
        }
        else{
            position = tableStatus.robot.pos;
        }
#endif
        convertAngularToAxial(lidarData, lidar_count, &position, 50);
        
        position_t pos_opponent;
        if (position_opponentV2(lidarData, lidar_count, position, &pos_opponent)){
            // If it's the first reading, initialize the filtered position
            if (first_reading)
            {
                pos_opponent_filtered.x = pos_opponent.x;
                pos_opponent_filtered.y = pos_opponent.y;
                first_reading = false;
            }
            else
            {
                // Apply the low-pass filter
                pos_opponent_filtered.x = alpha * pos_opponent.x + (1 - alpha) * pos_opponent_filtered.x;
                pos_opponent_filtered.y = alpha * pos_opponent.y + (1 - alpha) * pos_opponent_filtered.y;
            }

            // Save the filtered position to tableStatus
            tableStatus.pos_opponent.x = pos_opponent_filtered.x;
            tableStatus.pos_opponent.y = pos_opponent_filtered.y;

            opponentInAction(&tableStatus, &pos_opponent_filtered);
        }
    }
}

void EndSequence()
{

    LOG_DEBUG("Stopping");

    StopAPIServer();
    api_server_thread.join();

    arduino->moveStepper(0, 1);
#ifndef DISABLE_LIDAR
    GPIO_stopPWMMotor();
#endif
    arduino->servoPosition(4, 180);
    arduino->ledOff(2);
    arduino->ledOff(1);
    arduino->servoPosition(1, 180);
    arduino->moveStepper(0, 1);
    robotI2C->set_motor_state(false);
    robotI2C->set_brake_state(false);
    robotI2C->stop();
    lidarStop();
    sleep(1);
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
                LOG_GREEN_INFO("We are connected to a wifi : ", line.find("wlan0"));
                // Si la ligne contient "wlan0", cela indique que l'interface Wi-Fi est présente
                return true;
            }
        }
        file.close();
    }
    else
        LOG_ERROR("Erreur : Impossible d'ouvrir le fichier /proc/net/wireless.");
    return false;
}

void executePythonScript(const std::string &command)
{
    std::system(command.c_str());
}
