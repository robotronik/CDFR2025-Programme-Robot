#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <thread>
#include <unistd.h>  // for usleep

#include "main.hpp"
#include "actions/functions.h"
#include "lidar/lidarAnalize.h"
#include "utils/utils.h"
#include "utils/logger.hpp"
#include "restAPI/restAPI.hpp"
#include "navigation/highways.h"

#include "actions/actionContainer.hpp"

// #define DISABLE_LIDAR
// #define TEST_API_ONLY
#define DISABLE_LIDAR_BEACONS
// #define EMULATE_I2C


TableState tableStatus;

// Initiation of i2c devices
#ifndef EMULATE_I2C
Asserv asserv(I2C_ASSER_ADDR);
Arduino arduino(I2C_ARDUINO_ADDR);
#else
Asserv asserv(-1);
Arduino arduino(-1);
#endif

Lidar lidar;

main_State_t currentState;
main_State_t nextState;
bool initState;
actionContainer actionSystem;
bool manual_ctrl;
bool (*manual_currentFunc)(); //Pointer to a function to execute of type bool func(void)

std::thread api_server_thread;

// Prototypes
int StartSequence();
void GetLidarV2();
void EndSequence();

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

    // Private counter
    int sensorCount = 0;
    unsigned long loopStartTime;
    while (!ctrl_c_pressed)
    {
        LOG_SCOPE("Main");
        loopStartTime = _millis();

        // Get Sensor Data
        {
            int16_t x, y, theta;
            asserv.get_coordinates(x, y, theta);
            tableStatus.robot.pos = {x, y, theta};
            // LOG_GREEN_INFO("X = ", x," Y = ", y, " theta = ", theta);

            if (currentState != INIT)
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
                arduino.setStepper(0, 1);
                arduino.setStepper(0, 2);
                arduino.setStepper(0, 3);
                resetActionneur();
                sensorCount = 0;
                arduino.RGB_Rainbow();
            }

            bool initButtonState;
            arduino.readSensor(1, initButtonState);
            if (initButtonState){
                sensorCount++;
                if (sensorCount == 5)
                    nextState = WAITSTART;
            }
            else
                sensorCount = 0;            

            break;
        }
        //****************************************************************
        case WAITSTART:
        {
            if (initState){
                LOG_STATE("WAITSTART");    
                resetActionneur();
                // asserv.go_to_point(tableStatus.robot.pos.x, tableStatus.robot.pos.y);
                // asserv.set_motor_state(true);
                // asserv.set_brake_state(true); //TODO should be false
                //asserv.set_linear_max_speed(MAX_SPEED);
                //LOG_DEBUG("Waiting for get_command_buffer_size to be 0");
                //while(asserv.get_command_buffer_size() != 0); //wait end of all action above
                lidar.startSpin();            
                sensorCount = 0;
            }

            colorTeam_t color = readColorSensorSwitch();
            if (color != tableStatus.robot.colorTeam){
                LOG_INFO("Color switch detected");
                tableStatus.robot.colorTeam = color;

                switch (color)
                {
                case BLUE:
                    LOG_INFO("teams : BLUE");
                    nextState = WAITSTART;
                    asserv.set_coordinates(-770, -1390, 90);
                    arduino.RGB_Blinking(0, 0, 255);
                    break;
                case YELLOW:
                    LOG_INFO("teams : YELLOW");
                    nextState = WAITSTART;
                    asserv.set_coordinates(-770, 1390, -90);
                    arduino.RGB_Blinking(255, 255, 0);
                    break;
                case NONE:
                    arduino.RGB_Blinking(255, 0, 0);
                    break;
                }
            }
            bool magnetSensorState;
            arduino.readSensor(2, magnetSensorState);
            // Counts the number of time the magnet sensor
            if (magnetSensorState)
                sensorCount++;
            else
                sensorCount = 0;

            if (sensorCount == 5)
                nextState = RUN;
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
                actionSystem.initAction(&asserv, &arduino, &(tableStatus));
            }
            bool finished = actionSystem.actionContainerRun(&asserv, &tableStatus);

            if (_millis() > tableStatus.startTime + 90000 || tableStatus.FIN || finished)
            {
                nextState = FIN;
            }
            break;
        }


        //****************************************************************
        case MANUAL:
        {
            if (initState){
                LOG_STATE("MANUAL");
                arduino.RGB_Blinking(255, 0, 255); // Purple blinking
            }

            // Execute the function as long as it returns false
            if (manual_currentFunc != NULL && manual_currentFunc != nullptr){
                if (manual_currentFunc()){
                    manual_currentFunc = NULL;
                }
            }
            if (!manual_ctrl)
                nextState = FIN;
            break;
        }

        //****************************************************************
        case FIN:
        {
            LOG_STATE("FIN");
            arduino.moveServo(4, 180);
            arduino.moveServo(1, 180);
            arduino.disableStepper(1);
            asserv.set_motor_state(false);
            asserv.set_brake_state(false);
            lidar.stopSpin();
            nextState = INIT;
            break;
        }
        //****************************************************************
        default:
            LOG_STATE("default");
            nextState = INIT;
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

    arduino.RGB_Blinking(255, 0, 0); // Red blinking

#ifndef DISABLE_LIDAR
    if (!lidar.setup("/dev/ttyAMA0", 256000))
    {
        LOG_ERROR("Cannot find the lidar");
        return -1;
    }
#endif


    tableStatus.init();

    // LOG_SETROBOT(asserv);
    init_highways();

    // Start the api server in a separate thread
    api_server_thread = std::thread([&]()
                                    { StartAPIServer(); });

#ifdef TEST_API_ONLY
    TestAPIServer();
    // Wait for program termination
    int i = 0;
    while(!ctrl_c_pressed){
        sleep(0.1);
#ifndef DISABLE_LIDAR
        getData(lidarData, lidar_count);
#endif
        if (i % 10000 == 0){
            // randomly change the position of highway obstacles
            for (int i = 0; i < 1; i++){
                obs_obj_stocks[i].pos.x = rand() % 1500 - 750;
                obs_obj_stocks[i].pos.y = rand() % 2200 - 1100;
            }
            // and the pos of the opponent obstacle
            obs_obj_opponent.pos.x = rand() % 1500 - 750;
            obs_obj_opponent.pos.y = rand() % 2200 - 1100;
            tableStatus.pos_opponent.x = obs_obj_opponent.pos.x;
            tableStatus.pos_opponent.y = obs_obj_opponent.pos.y;

            // randomly change the position of the robot
            tableStatus.robot.pos.x = rand() % 1500 - 750;
            tableStatus.robot.pos.y = rand() % 2200 - 1100;

            // goto a random position
            navigationGoTo(rand() % 1500 - 750, rand() % 2200 - 1100, 0, Direction::FORWARD, Rotation::SHORTEST, Rotation::SHORTEST, true);
        }
        i++;
    }
    StopAPIServer();
    api_server_thread.join();
    return -1;
#endif

    currentState = INIT;
    nextState = INIT;
    initState = true;
    manual_ctrl = false;
    manual_currentFunc = NULL;

    actionSystem.init(&asserv, &arduino, &tableStatus);

    // std::string colorTest = tableStatus.colorTeam == YELLOW ? "YELLOW" : "BLUE";
    // std::filesystem::path exe_pathTest = std::filesystem::canonical(std::filesystem::path(argv[0])).parent_path();
    // std::filesystem::path python_script_pathTest = exe_pathTest / "../startPAMI.py";
    // std::string commandTest = "python3 " + python_script_pathTest.string() + " " +  colorTest;
    // std::thread python_threadTest(executePythonScript,commandTest);

    asserv.set_coordinates(0,0,0);

    LOG_INFO("Init sequence done");
    return 0;
}

void GetLidar()
{
    // Averages the position of the opponent over a few scans
    static position_t pos_opponent_avg_sum = {0, 0, 0};
    static int pos_opponent_avg_count = 0, count_pos = 0;

    if (lidar.getData())
    {
        position_t position = tableStatus.robot.pos;
        position_t pos_opponent = position;
        convertAngularToAxial(lidar.data, lidar.count, &position, -100);
        //init_position_balise(lidar.data, lidar_count, &position);
        convertAngularToAxial(lidar.data, lidar.count, &position, 50);
        if (position_opponent(lidar.data, lidar.count, position, &pos_opponent)){
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
                    opponentInAction(&pos_opponent);
                }
            }
        }


        if (count_pos == 10)
            count_pos = 0;
        count_pos++;

        //int16_t braking_distance = asserv.get_braking_distance();
        //tableStatus.robot.collide = collide(lidarData, lidar_count, braking_distance);
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

    if (lidar.getData())
    {
        position_t position;
        colorTeam_t color;
        // TODO : Add offset to lidar robot pos
#ifndef DISABLE_LIDAR_BEACONS
        if (position_robot_beacons(lidar.data, lidar.count, &position, tableStatus.robot.colorTeam, &color)){
            LOG_GREEN_INFO("Successfully found the robot's position using beacons");
            LOG_GREEN_INFO("X = ", position.x," Y = ", position.y, " theta = ", position.theta);
            // TODO, apply that new position to the tableStatus robot pos and the asserv
        }
        else{
            position = tableStatus.robot.pos;
        }
#endif
        convertAngularToAxial(lidar.data, lidar.count, &position, 50);
        
        position_t pos_opponent;
        if (position_opponentV2(lidar.data, lidar.count, position, &pos_opponent)){
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

            opponentInAction(&pos_opponent_filtered);
        }
    }
}

void EndSequence()
{
    LOG_DEBUG("Stopping");
    
    // Stop the API server
    StopAPIServer();
    api_server_thread.join();

    // Stop the lidar
#ifndef DISABLE_LIDAR
    lidar.Stop();
#endif

    asserv.set_motor_state(false);
    asserv.set_brake_state(false);
    //asserv.stop();

    arduino.RGB_Solid(0, 0, 0); // OFF

    resetActionneur();
    delay(1000);
    disableActionneur();

    LOG_DEBUG("Stopped");
}