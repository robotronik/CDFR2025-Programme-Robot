#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include "httplib.h"
#include "ArucoCam.hpp"
#include "logger.hpp"
#include "nlohmann/json.hpp" // For handling JSON
using json = nlohmann::json;
#define PORT_OFFSET 5000

pid_t startPythonProgram(const std::string &scriptPath, const std::string &args);
void stopPythonProgram(pid_t pid);

ArucoCam::ArucoCam(int cam_number, const std::string &calibration_file_path) {
    id = cam_number;
    if (id < 0) {
        pid=-1;
        LOG_INFO("Emulating ArucoCam");
        return;
    }
    pid = startPythonProgram("detect_aruco.py", "--api-port " + std::to_string(PORT_OFFSET + id) + " --cam " + std::to_string(id) + " " + calibration_file_path);
    if (pid == -1) {
        LOG_ERROR("Failed to start ArucoCam ", id);
    }
    else
    {
        LOG_GREEN_INFO("ArucoCam ", id, " started with PID ", pid);
    }
}
ArucoCam::~ArucoCam(){
    if (pid > 0)
        stopPythonProgram(pid);
}
bool ArucoCam::getPos(){
    LOG_DEBUG("ArucoCam::getPos()");
    // Calls /position rest api endpoint of the ArucoCam API
    // Returns true if the call was successful, false otherwise
    if (id < 0) {
        // TODO change this to return a random position
        return true;
    }
    const std::string url = "http://localhost:" + std::to_string(PORT_OFFSET + id);
    LOG_DEBUG("ArucoCam::getPos() - URL: ", url);
    // HTTP
    httplib::Client cli(url);
    auto res = cli.Get("/position");
    // Check for nullptr
    if (!res) {
        LOG_ERROR("ArucoCam::getPos() - Failed to fetch response");
        return false;
    }
    LOG_GREEN_INFO("status is ", res->status);
    LOG_GREEN_INFO("body is ", res->body);

    // Check if the response code is 200 (OK)
    if (res->status == 200) {
        try {
            // Parse JSON response
            json jsonResponse = json::parse(res->body);
            std::cout << "API Response: " << jsonResponse.dump(4) << std::endl;
            LOG_GREEN_INFO("aruco cam ", id, " - API Response: ", jsonResponse.dump(4));
            return true;
        } catch (const json::parse_error& e) {
            LOG_ERROR("aruco cam ", id, " - JSON parse error: ", e.what());
            return false;
        }
    } else {
        LOG_ERROR("aruco cam ", id, " - HTTP error: ", res->status);
        return false;
    }
}

pid_t startPythonProgram(const std::string &scriptPath, const std::string &args) {
    pid_t pid = fork();

    if (pid == -1) {
        LOG_ERROR("startPythonProgram - Failed to fork process");
        return -1;
    } else if (pid == 0) {
        // Child process: Split the args string into individual arguments
        std::vector<const char*> execArgs;
        execArgs.push_back("python3"); // Python interpreter
        execArgs.push_back(scriptPath.c_str()); // Script path
        execArgs.push_back("--api-port");
        execArgs.push_back("5000");
        execArgs.push_back("data/camera_calibration.yml");
        execArgs.push_back(NULL);
        // Execute the Python program with arguments
        execvp("python3", const_cast<char* const*>(execArgs.data()));
        LOG_ERROR("startPythonProgram - Failed to execute Python script");
        _exit(1); // Ensure child process exits
    }

    // Parent process: Return child PID
    return pid;
}

void stopPythonProgram(pid_t pid) {
    if (pid <= 0) {
        LOG_ERROR("stopPythonProgram - Invalid PID");
        return;
    }

    if (kill(pid, SIGTERM) == -1) {
        LOG_ERROR("stopPythonProgram - Failed to terminate process with PID ", pid);
    } else {
        LOG_INFO("stopPythonProgram - Process with PID ", pid, " terminated");
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    if (response) {
        response->append(static_cast<char*>(contents), totalSize);
    }
    return totalSize;
}