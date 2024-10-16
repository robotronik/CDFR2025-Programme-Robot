#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.hpp"
#include "math.h"
#include "logger.hpp"

#define MAP(value, fromLow, fromHigh, toLow, toHigh) ((toLow) + (((value) - (fromLow)) * ((toHigh) - (toLow)) / ((fromHigh) - (fromLow))))
typedef struct {
    int i ; //indice début élément décord
    int nb; //nombre d'élement 
    float moy_dist ; //somme distance
    float moy_angle; //somme angle
    float cm; //taille object
} element_decord;

void convertAngularToAxial(lidarAnalize_t* data, int count, position_t *position,int narrow);

void position_ennemie(lidarAnalize_t* data, int count, position_t *position);

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


