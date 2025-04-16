#include "utils/logger.hpp"
#include <iostream>

namespace SimpleLogger {
    std::map<std::string, int> logFiles;
    int globalLogNum = -1;
    bool isInMatch = false;

    void changeMatchState(bool match) {
        std::cout << "INNNNMATCH " << match << std::endl;
        isInMatch = match;
        std::cout << "INNNNMATCH " << isInMatch << std::endl;
    }

    // autres fonctions
}