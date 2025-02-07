#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <cstdlib>
#include <string>
#include <fstream>

typedef enum {
    NONE,
    BLUE,
    YELLOW
} colorTeam_t;

inline unsigned long _millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Utilisation de CLOCK_MONOTONIC pour éviter les changements d'horloge
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000); // Conversion en millisecondes
}

inline char* currentTimeFormatted() {
    time_t temps;
    struct tm date;
    char* tempsFormate = (char*)malloc(80);  // dynamically allocate memory
    if (tempsFormate == NULL) return NULL;  // check for allocation failure
    time(&temps);
    date = *localtime(&temps);
    strftime(tempsFormate, 80, "%Y-%m-%d %H:%M:%S", &date);
    return tempsFormate;
}

inline bool isWifiConnected()
{
    std::ifstream file("/proc/net/wireless");
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.find("wlan0") != std::string::npos)
            {
                return true;
            }
        }
        file.close();
    }
    return false;
}

inline void executePythonScript(const std::string &command)
{
    std::system(command.c_str());
}

#endif