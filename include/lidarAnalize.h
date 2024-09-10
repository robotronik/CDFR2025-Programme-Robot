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

void pixelArtPrint(lidarAnalize_t* data, int count,int sizeX,int sizeY,int scale,position_t position);

double distance_2_pts(double d1,double deg1, double d2, double deg2);

void supprimerElement(element_decord**& array, int& rows, int index);

void sol_eq_2cercle(double xA,double  yA,double AM,double xB,double yB,double BM,double xC, double yC, double CM, double *xM, double *yM);

void position_facile(lidarAnalize_t* data,int count, double *X, double* Y, double X_prec, double Y_prec);

void init_position_balise(lidarAnalize_t* data, int count, position_t *position);


