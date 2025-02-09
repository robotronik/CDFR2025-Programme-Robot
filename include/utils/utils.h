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

inline std::string currentTimeFormatted() {
    time_t temps;
    struct tm date;
    char buffer[32];
    time(&temps);
    date = *localtime(&temps);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &date);
    return std::string(buffer);
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

#endif