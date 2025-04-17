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

#include <vector>
#include <optional>
#include <sstream>
#include <string>
#include <initializer_list>

#ifdef __CROSS_COMPILE_ARM__
    #define LOG_PATH "/home/robotronik/LOG_CDFR2025/"
#else
    #define LOG_PATH "log/"
#endif

enum class Color {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    GRAY = 90
};

enum class Background {
    BLACK = 40,
    RED = 41,
    GREEN = 42,
    YELLOW = 43,
    BLUE = 44,
    MAGENTA = 45,
    CYAN = 46,
    WHITE = 47
};

enum class Style {
    RESET = 0,
    BOLD = 1,
    FAINT = 2,
    ITALIC = 3,
    UNDERLINE = 4,
    BLINK = 5,
    REVERSE = 7
};


struct AnsiStyle {
    std::optional<Color> color = std::nullopt;
    std::optional<Background> background = std::nullopt;
    std::vector<Style> styles = {};

    AnsiStyle() = default;

    AnsiStyle(std::optional<Color> c,
              std::optional<Background> bg = std::nullopt,
              std::initializer_list<Style> s = {})
        : color(c), background(bg), styles(s) {}
};

class logger
{
private:
    static int globalLogNum;
    bool logStatus = true;
    std::string baseName;
    int fileDescriptor = -1;

    static std::string getExecutablePath();
    static std::string resolveLogPath(const std::string& path);
    static void        ensureLogDirectoryAndNumFile(const std::string& logDir, const std::string& numLogFile);
    static int         readAndIncrementLogNum(const std::string& fullPath);

    static std::string makeAnsiCode(
        std::optional<Color> color = std::nullopt,
        std::optional<Background> bg = std::nullopt,
        const std::vector<Style>& styles = {}
    );

    int getLogNumber();
    int getLogFileDescriptor();

    void logToFile(const std::string& message, bool forceSync);
    void logToOutput(const std::string& message);
public:
    logger(const std::string name, bool defaultlogStatus = true);
    ~logger();

    void log(bool logFile,
         bool forceSync,
         bool logOut,
         const AnsiStyle& style,
         const std::string& message);

    int getLogID();
    void setLogStatus(bool logS);
};


namespace SimpleLogger {


// Base case for message appending: when there are no more arguments.
inline void appendMessage(std::ostringstream& /*oss*/) {}

// Recursively append all parts of the message.
template<typename T, typename... Args>
inline void appendMessage(std::ostringstream& oss, const T& value, const Args&... args) {
    oss << value;
    appendMessage(oss, args...);
}

template<typename... Args>
inline void log(logger loggerType, bool logFile, bool forceSync, bool logOut, const AnsiStyle& style,const std::string& typeLevel, const std::string& file, const int line, const std::string& message, const Args&... args) {

    std::ostringstream oss;
    appendMessage(oss, message, args...);

    std::ostringstream playLoad;
            playLoad << currentTimeFormatted() << " "
            << std::left << std::setw(10) << ("[" + typeLevel + "]") // Fixed width for log level
            << std::left << std::setw(30) << ("[" + file + ":" + std::to_string(line) + "]") << "  " // Fixed width for function name
            << oss.str();

    loggerType.log(logFile,forceSync,logOut,style,playLoad.str());
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

extern logger log_asserv;
extern logger log_main;
// Convenience macros that automatically pass the calling function's name.
#define LOG_DEBUG(message, ...)      SimpleLogger::log(log_main,true,false,true,AnsiStyle(Color::GREEN),"DEBUG", __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...)       SimpleLogger::log(log_main,true,false,true,AnsiStyle(Color::GREEN),"INFO", __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)    SimpleLogger::log(log_main,true,false,true,AnsiStyle(Color::GREEN),"WARNING", __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)      SimpleLogger::log(log_main,true,false,true,AnsiStyle(Color::GREEN),"ERROR", __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_GREEN_INFO(message, ...) SimpleLogger::log(log_main,true,false,true,AnsiStyle(Color::GREEN),"GREEN", __FILE__, __LINE__, message, ##__VA_ARGS__)

#define LOG_ASSERV_SET_INFO(message, ...) SimpleLogger::log(log_asserv,true,true,false,AnsiStyle(Color::GREEN),"I2C", __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_ASSERV_GET_INFO(message, ...) SimpleLogger::log(log_asserv,true,false,false,AnsiStyle(Color::GREEN),"I2C", __FILE__, __LINE__, message, ##__VA_ARGS__)

#define LOG_INIT() initLog();

#define LOG_GET_NUMBER() log_asserv.getLogID()

#define LOG_MATCH_STATE(stat) log_asserv.setLogStatus(stat)