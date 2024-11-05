#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.hpp"
#include "math.h"
#include "logger.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define MAP(value, fromLow, fromHigh, toLow, toHigh) ((toLow) + (((value) - (fromLow)) * ((toHigh) - (toLow)) / ((fromHigh) - (fromLow))))
typedef struct {
    int i ; //indice début élément décord
    int nb; //nombre d'élement 
    float moy_dist ; //somme distance
    float moy_angle; //somme angle
    float cm; //taille object
} element_decord;

void convertAngularToAxial(lidarAnalize_t* data, int count, position_t *position,int narrow);

bool position_opponent(lidarAnalize_t* data, int count, position_t robot_pos, position_t *opponent_pos);
bool position_opponentV2(lidarAnalize_t* data, int count, position_t robot_pos, position_t *opponent_pos);

bool collideFordward(lidarAnalize_t* data, int count);

bool collideBackward(lidarAnalize_t* data, int count); 

int collide(lidarAnalize_t* data, int count ,int distanceStop);

void printLidarAxial(lidarAnalize_t* data, int count);

void printAngular(lidarAnalize_t* data, int count);

double distance_2_pts(double d1,double deg1, double d2, double deg2);

void supprimerElement(element_decord**& array, int& rows, int index);

double angle(position_float_t A, position_float_t B, position_float_t C);
int in_table(position_float_t M);
position_float_t sol_eq_2cercle(position_float_t A, double RA, position_float_t B, double RB);

position_float_t position_estime(double angle12, double angle23, double angle31,double angle_poto2, int d12, int d23,int d31);
position_float_t position_estime2(double alpha12, double alpha23, double alpha31);
void init_position_balise(lidarAnalize_t* data, int count, position_t *position);


// Ludovic Bouchard - beacons v2
#define BEACONS_COUNT 3

typedef struct {
    bool valid;
    int beacon_count = BEACONS_COUNT;
    position_double_t beacons_rel_pos[BEACONS_COUNT];
    double diameters[BEACONS_COUNT];
} beacon_detection_t;

extern beacon_detection_t beacon_detection;
void to_json(json& j, const beacon_detection_t& bd);

bool transform_coordinates(double x1, double y1, double theta1, double x1_prime, double y1_prime, double theta1_prime, double* x, double* y, double* theta);
int delta_angle(int angle1, int angle2);
double delta_angle_double(double angle1, double angle2);
bool position_robot_beacons(lidarAnalize_t* data, int count, position_t *position, colorTeam_t team_color, colorTeam_t* out_team_color);