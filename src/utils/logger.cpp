#include "utils/logger.hpp"
#include <iostream>

//logger instance
logger* log_main() {
    static logger* instance = new logger("mainLog", true);
    return instance;
}

logger* log_asserv() {
    static logger* instance = new logger("asserv", false);
    return instance;
}


int logger::globalLogNum = -1;

logger::logger(const std::string name, bool defaultlogStatus){
    logEnable = defaultlogStatus;
    baseName = name;
    stdOutInitValid = false;
    timeString = currentTimeFormatted();
}

logger::~logger()
{
}

std::string logger::init(bool withColor){
    std::ostringstream intiMessage;
    if(withColor)
        intiMessage << "\033[1;31m";
    intiMessage << "  _____   ____  ____   ____ _______ _____   ____  _   _ _____ _  __" << std::endl;
    intiMessage << " |  __ \\ / __ \\|  _ \\ / __ \\__   __|  __ \\ / __ \\| \\ | |_   _| |/ /" << std::endl;
    intiMessage << " | |__) | |  | | |_) | |  | | | |  | |__) | |  | |  \\| | | | | ' / " << std::endl;
    intiMessage << " |  _  /| |  | |  _ <| |  | | | |  |  _  /| |  | | .   | | | |  <  " << std::endl;
    intiMessage << " | | \\ \\| |__| | |_) | |__| | | |  | | \\ \\| |__| | |\\  |_| |_| . \\ " << std::endl;
    intiMessage << " |_|  \\_\\\\____/|____/ \\____/  |_|  |_|  \\_\\\\____/|_| \\_|_____|_|\\_\\" << std::endl;
    if(withColor)
        intiMessage << "\n\033[0m";

    intiMessage << "ROBOTRONIK" << std::endl;
    intiMessage << "PROGRAM ROBOT CDFR" << std::endl;
    intiMessage << "Start Time : " << timeString << std::endl;
    return intiMessage.str();
}

std::string logger::getExecutablePath(){
    char result[1000];
    ssize_t count = readlink("/proc/self/exe", result, sizeof(result));
    if (count == -1) {
        throw std::runtime_error("Unable to determine executable path");
    }
    return std::filesystem::path(std::string(result, count)).parent_path().string();
}

std::string logger::resolveLogPath(const std::string& path) {
    std::filesystem::path p(path);
    if (p.is_absolute()) {
        return path;
    }
    return (getExecutablePath() / p).string();
}

void logger::ensureLogDirectoryAndNumFile(const std::string& logDir, const std::string& numLogFile) {
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

int logger::readAndIncrementLogNum(const std::string& fullPath) {
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

int logger::getLogNumber(){
    if (globalLogNum == -1) {
        globalLogNum = readAndIncrementLogNum(resolveLogPath(LOG_PATH));
    }
    return globalLogNum;
}

int logger::getLogID(){
    return globalLogNum;
}

void logger::setLogStatus(bool logS){
    logEnable = logS;
}

int logger::getLogFileDescriptor() {
    if(fileDescriptor != -1)
        return fileDescriptor;

    std::ostringstream oss;
    oss << resolveLogPath(LOG_PATH) << baseName << "_" << getLogNumber() << ".log";
    std::string fullName = oss.str();

    fileDescriptor = open(fullName.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fileDescriptor == -1) {
        perror(("Failed to open log file: " + fullName).c_str());
        return -1;
    }

    std::string initString = init();
    write(fileDescriptor, initString.c_str(), initString.size());

    return fileDescriptor;
}

void logger::logToFile(const std::string& message, bool forceSync){
    int fd = getLogFileDescriptor();
    if (fd == -1) return;

    std::string fullMessage = message + "\n";
    write(fd, fullMessage.c_str(), fullMessage.size());

    if (forceSync) {
        fsync(fd);
    }
}

std::string logger::makeAnsiCode(
    std::optional<Color> color,
    std::optional<Background> bg,
    const std::vector<Style>& styles
) {
    std::ostringstream oss;
    oss << "\033[";

    bool first = true;

    for (Style style : styles) {
        if (!first) oss << ";";
        oss << static_cast<int>(style);
        first = false;
    }

    if (color.has_value()) {
        if (!first) oss << ";";
        oss << static_cast<int>(*color);
        first = false;
    }

    if (bg.has_value()) {
        if (!first) oss << ";";
        oss << static_cast<int>(*bg);
    }

    oss << "m";
    return oss.str();
}

void logger::logToOutput(const std::string& message){
    if (stdOutInitValid == false) {
        stdOutInitValid = true;
        std::cout << init(true);
    }
    std::cout << message;
    std::cout.flush();
}

void logger::log(bool logFile,
         bool forceSync,
         bool logOut,
         const AnsiStyle& style,
         const std::string& message) {

    std::ostringstream formatted;

    if (logOut && logEnable) {
        formatted << makeAnsiCode(style.color, style.background, style.styles)<< message  << "\033[0m" << std::endl;
        logToOutput(formatted.str());
    }

    if (logFile && logEnable) {
        logToFile(message,forceSync);
    }
}