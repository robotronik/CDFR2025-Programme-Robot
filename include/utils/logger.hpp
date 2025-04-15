#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <exception>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <filesystem>
#include <sys/stat.h>
#include "utils/utils.h"

enum class LogLevel {
    I2C,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    GREENINFO
};

// Only messages with a level equal to or above CURRENT_LOG_LEVEL will be printed.
constexpr LogLevel CURRENT_LOG_LEVEL = LogLevel::DEBUG;

namespace SimpleLogger {

#ifdef __CROSS_COMPILE_ARM__
    #define LOG_PATH "/home/robotronik/LOG_CDFR2025/"
#else
    #define LOG_PATH "log/"
#endif

inline static std::map<std::string, int> logFiles;
inline static bool initOnceFlag = false;
inline static int globalLogNum = -1;

inline std::string getExecutablePath() {
    char result[1000];
    ssize_t count = readlink("/proc/self/exe", result, sizeof(result));
    if (count == -1) {
        throw std::runtime_error("Unable to determine executable path");
    }
    return std::filesystem::path(std::string(result, count)).parent_path().string();
}

inline std::string resolveLogPath(const std::string& path) {
    std::filesystem::path p(path);
    if (p.is_absolute()) {
        return path;
    }
    return (getExecutablePath() / p).string();
}

inline void ensureLogDirectoryAndNumFile(const std::string& logDir, const std::string& numLogFile) {
    struct stat st;
    if (stat(logDir.c_str(), &st) != 0) {
        if (mkdir(logDir.c_str(), 0755) != 0) {
            perror("Failed to create log directory");
            exit(EXIT_FAILURE);
        }
        std::ofstream out(numLogFile);
        if (!out) {
            std::cerr << "Erreur : impossible de créer le fichier " << numLogFile << std::endl;
            exit(EXIT_FAILURE);
        }
        out << 0;
        out.close();
    } else {
        std::ifstream in(numLogFile);
        if (!in) {
            std::cerr << "Erreur : le dossier '" << logDir << "' existe mais pas le fichier 'numLog'. Abandon." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

inline int readAndIncrementLogNum(const std::string& fullPath) {
    std::string numFilePath = fullPath+"/numLog";
    ensureLogDirectoryAndNumFile(fullPath,numFilePath);
    std::ifstream in(numFilePath);
    int num = 0;
    if (in >> num) {
        in.close();
    }

    std::ofstream out(numFilePath, std::ios::trunc);
    out << (num + 1);
    return num;
}

inline int getLogFileDescriptor(const std::string& baseName, const std::string& logPath = "log/") {
    auto it = logFiles.find(baseName);
    if (it != logFiles.end()) {
        return it->second;
    }

    std::string fullPath = resolveLogPath(logPath);

    if(initOnceFlag == false){
        globalLogNum = readAndIncrementLogNum(fullPath);
        initOnceFlag = true;
    }

    std::ostringstream oss;
    oss << fullPath << baseName << "_" << globalLogNum << ".log";
    std::string fullName = oss.str();

    int fd = open(fullName.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror(("Failed to open log file: " + fullName).c_str());
        return -1;
    }

    logFiles[baseName] = fd;
    return fd;
}

inline void logToFile(const std::string& baseName, const std::string& message, bool forceSync) {
    int fd = getLogFileDescriptor(baseName,LOG_PATH);
    if (fd == -1) return;

    std::string fullMessage = message + "\n";
    write(fd, fullMessage.c_str(), fullMessage.size());

    if (forceSync) {
        fsync(fd);
    }
}

inline std::string getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::I2C:     return "I2C";
        case LogLevel::DEBUG:     return "DEBUG";
        case LogLevel::INFO:      return "INFO";
        case LogLevel::WARNING:   return "WARNING";
        case LogLevel::ERROR:     return "ERROR";
        case LogLevel::GREENINFO: return "INFO";
        default:                  return "";
    }
}

inline std::string getColorCode(LogLevel level) {
    switch (level) {
        case LogLevel::I2C:       return "\033[90m"; // Light gray
        case LogLevel::DEBUG:     return "\033[90m"; // Light gray
        case LogLevel::GREENINFO: return "\033[32m"; // Green
        case LogLevel::WARNING:   return "\033[33m"; // Yellow
        case LogLevel::ERROR:     return "\033[31m"; // Red
        default:                  return "\033[0m";  // Default/Reset
    }
}

// Base case for message appending: when there are no more arguments.
inline void appendMessage(std::ostringstream& /*oss*/) {}

// Recursively append all parts of the message.
template<typename T, typename... Args>
inline void appendMessage(std::ostringstream& oss, const T& value, const Args&... args) {
    oss << value;
    appendMessage(oss, args...);
}

static std::string previousMessage;
static unsigned long previousMessageTime;

template<typename... Args>
inline void log(LogLevel level, const std::string& functionName, const int line, const std::string& file, bool force, const std::string& message, const Args&... args) {

    bool stdOutNotValid = (level < CURRENT_LOG_LEVEL) || (message == previousMessage && _millis() / 1000 == previousMessageTime);

    if(stdOutNotValid && file.empty())
        return;

    std::ostringstream oss;
    appendMessage(oss, message, args...);

    std::ostringstream playLoad;
            playLoad << currentTimeFormatted() << " "
            << std::left << std::setw(10) << ("[" + getLevelString(level) + "]") // Fixed width for log level
            << std::left << std::setw(30) << ("[" + functionName + ":" + std::to_string(line) + "]") << "  " // Fixed width for function name
            << oss.str();

    if(!stdOutNotValid){
        std::string colorCode = getColorCode(level);
        std::string resetCode = "\033[0m";

        std::ostringstream logStream;
        logStream << colorCode
                << playLoad.str()
                << resetCode << std::endl;

        // Print the fully constructed log message in one operation.
        std::cout << logStream.str();
        std::cout.flush();  // Explicit flush to ensure immediate writing

        previousMessageTime = _millis() / 1000;
        previousMessage = message;
    }

    if (!file.empty()) {
        logToFile(file, playLoad.str(), force);
    }
}

} // namespace SimpleLogger

inline void initLog(void){
    std::cout << "\033[1;31m";
    std::cout << "  _____   ____  ____   ____ _______ _____   ____  _   _ _____ _  __" << std::endl;
    std::cout << " |  __ \\ / __ \\|  _ \\ / __ \\__   __|  __ \\ / __ \\| \\ | |_   _| |/ /" << std::endl;
    std::cout << " | |__) | |  | | |_) | |  | | | |  | |__) | |  | |  \\| | | | | ' / " << std::endl;
    std::cout << " |  _  /| |  | |  _ <| |  | | | |  |  _  /| |  | | .   | | | |  <  " << std::endl;
    std::cout << " | | \\ \\| |__| | |_) | |__| | | |  | | \\ \\| |__| | |\\  |_| |_| . \\ " << std::endl;
    std::cout << " |_|  \\_\\\\____/|____/ \\____/  |_|  |_|  \\_\\\\____/|_| \\_|_____|_|\\_\\" << std::endl;
    std::cout << "\n\033[0m";

    std::cout << "ROBOTRONIK" << std::endl;
    std::cout << "PROGRAM ROBOT CDFR" << std::endl;
    std::cout << "Start Time : " << currentTimeFormatted() << std::endl;
}


// Convenience macros that automatically pass the calling function's name.
#define LOG_DEBUG(message, ...)      SimpleLogger::log(LogLevel::DEBUG,    __FILE__, __LINE__, "", false, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...)       SimpleLogger::log(LogLevel::INFO,     __FILE__, __LINE__, "", false, message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)    SimpleLogger::log(LogLevel::WARNING,  __FILE__, __LINE__, "", false, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)      SimpleLogger::log(LogLevel::ERROR,    __FILE__, __LINE__, "", false, message, ##__VA_ARGS__)
#define LOG_GREEN_INFO(message, ...) SimpleLogger::log(LogLevel::GREENINFO,__FILE__, __LINE__, "", false, message, ##__VA_ARGS__)

#define LOG_ASSERV_SET_INFO(message, ...) SimpleLogger::log(LogLevel::I2C, __FILE__, __LINE__, "Asserv", true, message, ##__VA_ARGS__)
#define LOG_ASSERV_GET_INFO(message, ...) SimpleLogger::log(LogLevel::I2C, __FILE__, __LINE__, "Asserv", false, message, ##__VA_ARGS__)

#define LOG_INIT() initLog();