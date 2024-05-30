#pragma once

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155
#define ROBOT_Y_OFFSET 60

typedef struct 
{
    int   x;
    int   y;
    int   teta;
    int   time;
}position_t;

typedef struct 
{
    bool   valid;
    double   angle;
    double   dist;
    int   x;
    int   y;
    bool   onTable;
}lidarAnalize_t;