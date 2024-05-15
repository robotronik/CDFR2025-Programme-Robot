#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <asser.hpp>
#include <stack>

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, GREENINFO};


class ScopeLogger {
private:
    static std::stack<std::string> functionName_; 

public:
    ScopeLogger(const std::string& functionName){
        functionName_.push(functionName);
    }

    ~ScopeLogger() {
        functionName_.pop();
    }

    static std::string logIndentation(){
        std::ostringstream returnstring;
        if (!functionName_.empty()) {
            returnstring << "   "<< std::setw(15) <<  std::left << functionName_.top();
        }
        for (size_t i = 1; i < functionName_.size(); ++i) {
            returnstring << "=";
        }
        if (!functionName_.empty()) {
            returnstring << ">";
        }
        return returnstring.str();
    }

};

inline std::stack<std::string> ScopeLogger::functionName_;



class Logger {
private:
    Asser* robot = NULL;
    bool noLog = false;

public:

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void logSetRobot(Asser* robot_in){
        robot = robot_in;
    }

    void initLog(void){
        std::cout << "\033[1;31m";
        std::cout << "  _____   ____  ____   ____ _______ _____   ____  _   _ _____ _  __" << std::endl;
        std::cout << " |  __ \\ / __ \\|  _ \\ / __ \\__   __|  __ \\ / __ \\| \\ | |_   _| |/ /" << std::endl;
        std::cout << " | |__) | |  | | |_) | |  | | | |  | |__) | |  | |  \\| | | | | ' / " << std::endl;
        std::cout << " |  _  /| |  | |  _ <| |  | | | |  |  _  /| |  | | . ` | | | |  <  " << std::endl;
        std::cout << " | | \\ \\| |__| | |_) | |__| | | |  | | \\ \\| |__| | |\\  |_| |_| . \\ " << std::endl;
        std::cout << " |_|  \\_\\\\____/|____/ \\____/  |_|  |_|  \\\\_\\____/|_| \\_|_____|_|\\_\\" << std::endl;
        std::cout << "\033[0m";                                                           
                                                                    
        std::cout << "ROBOTRONIK" << std::endl;
        std::cout << "PROGRAM ROBOT CDFR" << std::endl;
        time_t temps;
        struct tm date;
        char tempsFormate[80];
        time(&temps);
        date = *localtime(&temps);
        strftime(tempsFormate, sizeof(tempsFormate), "%Y-%m-%d %H:%M:%S", &date);
        std::cout << "Start Time : " << tempsFormate << std::endl;
    }

    std::string getPosition(void){
        int x,y,teta;
        std::ostringstream returnstring;
        if(robot!=NULL && !noLog){
            noLog = true;
            robot->getCoords(x,y,teta);
            noLog = false;
            returnstring << "[" << std::setw(5) << x << " " << std::setw(5) << y << " " << std::setw(4) << teta <<"]";
        }        
        return returnstring.str();
    }

    template<typename... Args>
    void log(LogLevel level, const std::string& message, Args... args) {
        std::ostringstream oss;
        appendMessage(oss, message, args...);

        switch (level) {
            case LogLevel::DEBUG:
                std::cout << "[DEBUG]" << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation() << " " << oss.str() << std::endl;
                break;
            case LogLevel::INFO:
                std::cout << "[INFO] " << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " " << oss.str() << std::endl;
                break;
            case LogLevel::WARNING:
                std::cout << "[WARNING] " << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                break;
            case LogLevel::ERROR:
                std::cout << "\033[1;31m";
                std::cout << "[ERROR] " << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                std::cout << "\033[0m";
                break;
            case LogLevel::GREENINFO:
                std::cout << "\033[32m";
                std::cout << "[INFO] " << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                std::cout << "\033[0m";
                break;

            default:
                break;
        }
    }

private:
    Logger() {}

    template<typename T, typename... Args>
    void appendMessage(std::ostringstream& oss, const T& value, Args... args) {
        oss << value;
        appendMessage(oss, args...);
    }

    void appendMessage(std::ostringstream& oss) {}  // Base case for recursion
};





// Macros for easier logging
#define LOG_DEBUG(message, ...) Logger::getInstance().log(LogLevel::DEBUG, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...) Logger::getInstance().log(LogLevel::INFO, message, ##__VA_ARGS__)
#define LOG_GREEN_INFO(message, ...) Logger::getInstance().log(LogLevel::GREENINFO, message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...) Logger::getInstance().log(LogLevel::WARNING, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) Logger::getInstance().log(LogLevel::ERROR, message, ##__VA_ARGS__)
#define LOG_INIT() Logger::getInstance().initLog()
#define LOG_SETROBOT(robot) Logger::getInstance().logSetRobot(robot)
#define LOG_SCOPE(message) ScopeLogger __logger__(message)
#define LOG_STATE(message, ...) Logger::getInstance().log(LogLevel::INFO, message, ##__VA_ARGS__)
