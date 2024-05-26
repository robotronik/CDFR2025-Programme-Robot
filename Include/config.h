#pragma once
#include "utils.h"

#define NOMBRE_ACTIONS 4
#define LARGEUR_ROBOT 320
#define SECURITE 1
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* fdatasync(2) is available */
/* #undef HAVE_FDATASYNC */

/* futimes(2) is available */
#define HAVE_FUTIMES 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* pread(2) and pwrite(2) are available */
#define HAVE_PREADWRITE 1

/* readahead(2) is available (linux) */
/* #undef HAVE_READAHEAD */

/* sendfile(2) is available and supported */
#define HAVE_SENDFILE 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* sync_file_range(2) is available */
/* #undef HAVE_SYNC_FILE_RANGE */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "libeio"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.0"

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155
#define ROBOT_Y_OFFSET 60

//Structure contenant toutes les informations sur une position
// int   x;
// int   y;
// int   teta;// téta d'un robot
// int   time;// date des données
typedef struct 
{
    int   x;// x
    int   y;// y
    int   teta;// téta d'un robot
    int   time;// date des données
}position_t;

typedef struct
{
    int x,y;
    float teta;
} vect_t;

//structure contenant toutes les informations sur un robot
// colorTeam_t color; équipe du robot
// position_t pos; position
// double vit; vitesse instantannée
// double vit_moy; vitesse moyenne
// bool robotHavePlante; le robot a des plantes
typedef struct
{
    int centre_x,centre_y;
    int demie_longueur,demie_largeur;
    int teta;
    forme_type_t forme;
}obstacle_t;
typedef struct
{
    colorTeam_t color;// équipe du robot
    position_t pos; //position du robot
    double vit_x,vit_y; //vitesse instantannée du robot
    bool robotHavePlante;// le robot a des plantes
    obstacle_t obstacle;
}robot_t;

//Structure contenant un point obtenu par traitement des données du lidar
// bool   valid; Si data valable
// double   angle; angle par rapport au robot
// double   dist; distance par rapport au robot
// int   x; x du point dans le référentiel de la table
// int   y; y du point dans le référentiel de la table
// bool   onTable;  si le point est sur la table
typedef struct 
{
    bool   valid;
    double   angle;//angle par rapport au robot
    double   dist;//distance par rapport au robot
    int   x;// x du point dans le référentiel de la table
    int   y;// y du point dans le référentiel de la table
    bool   onTable;// si le point est sur la table
}lidarAnalize_t;


typedef enum {
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    START,
    RUN,
    RETURNHOME,
    FIN,
    STOP
} main_State_t;