#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <exception>
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
inline void log(LogLevel level, const std::string& functionName, const int line, const std::string& message, const Args&... args) {
    if (level < CURRENT_LOG_LEVEL)
        return;
    if (message == previousMessage && _millis() / 1000 == previousMessageTime)
        return;
    std::ostringstream oss;
    appendMessage(oss, message, args...);

    std::string colorCode = getColorCode(level);
    std::string resetCode = "\033[0m";

    std::ostringstream logStream;
    logStream << colorCode
            << currentTimeFormatted() << " "
            << std::left << std::setw(10) << ("[" + getLevelString(level) + "]") // Fixed width for log level
            << std::left << std::setw(30) << ("[" + functionName + ":" + std::to_string(line) + "]") << "  " // Fixed width for function name
            << oss.str()
            << resetCode << std::endl;

    // Print the fully constructed log message in one operation.
    std::cout << logStream.str();
    std::cout.flush();  // Explicit flush to ensure immediate writing

    previousMessageTime = _millis() / 1000;
    previousMessage = message;
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
#define LOG_DEBUG(message, ...)      SimpleLogger::log(LogLevel::DEBUG,    __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...)       SimpleLogger::log(LogLevel::INFO,     __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)    SimpleLogger::log(LogLevel::WARNING,  __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)      SimpleLogger::log(LogLevel::ERROR,    __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_GREEN_INFO(message, ...) SimpleLogger::log(LogLevel::GREENINFO,__FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_ASSERV_SET_INFO(message, ...) SimpleLogger::log(LogLevel::I2C, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_ASSERV_GET_INFO(message, ...) SimpleLogger::log(LogLevel::I2C, __FILE__, __LINE__, message, ##__VA_ARGS__)

#define LOG_INIT() initLog();