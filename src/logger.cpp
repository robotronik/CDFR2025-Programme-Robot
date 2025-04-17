#include "utils/logger.hpp"
#include <iostream>

int logger::globalLogNum = -1;
logger log_asserv = logger("asserv", false);
logger log_main = logger("mainLog", false);
//asserv.log(bool logFile, bool forceFileSync, bool logOut, STYLE, __FILE__, __LINE__, message, ##__VA_ARGS__)

logger::logger(const std::string name, bool defaultlogStatus){
    logStatus = defaultlogStatus;
    baseName = name;
}

logger::~logger()
{
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
    logStatus = logS;
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
    std::cout << message;
    std::cout.flush();
}

void logger::log(bool logFile,
         bool forceSync,
         bool logOut,
         const AnsiStyle& style,
         const std::string& message) {

    std::ostringstream formatted;

    if (logOut) {
        formatted << makeAnsiCode(style.color, style.background, style.styles)<< message  << "\033[0m" << std::endl;
        logToOutput(formatted.str());
    }

    if (logFile) {
        logToFile(message,forceSync);
    }
}