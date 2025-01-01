#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <curl/curl.h>
#include "ArucoCam.hpp"
#include "logger.hpp"
#include "nlohmann/json.hpp" // For handling JSON
using json = nlohmann::json;

#define PORT_OFFSET 5000

// Callback to write the response data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);

pid_t startPythonProgram(const std::string &scriptPath);
void stopPythonProgram(pid_t pid);

ArucoCam::ArucoCam(int cam_number) {
    id = cam_number;
    pid = startPythonProgram("detect_aruco.py api-port " + std::to_string(PORT_OFFSET + id) + " cam " + std::to_string(id));
    if (pid == -1) {
        LOG_ERROR("Failed to start ArucoCam ", id);
    }
}
ArucoCam::~ArucoCam(){
    stopPythonProgram(pid);
}
bool ArucoCam::getPos(){
    // Calls /position rest api endpoint of the ArucoCam API
    // Returns true if the call was successful, false otherwise

    const std::string url = "http://localhost:" + std::to_string(PORT_OFFSET + id) + "/position";
    std::string responseBody;

    // Initialize a CURL handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG_ERROR("aruco cam ", id, " - Failed to initialize CURL");
        return false;
    }

    // Set CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LOG_ERROR("aruco cam ", id, " - CURL error: ", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return false;
    }

    // Get the HTTP response code
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    // Clean up CURL
    curl_easy_cleanup(curl);

    // Check if the response code is 200 (OK)
    if (response_code == 200) {
        try {
            // Parse JSON response
            json jsonResponse = json::parse(responseBody);
            std::cout << "API Response: " << jsonResponse.dump(4) << std::endl;
            LOG_GREEN_INFO("aruco cam ", id, " - API Response: ", jsonResponse.dump(4));
            return true;
        } catch (const json::parse_error& e) {
            LOG_ERROR("aruco cam ", id, " - JSON parse error: ", e.what());
            return false;
        }
    } else {
        LOG_ERROR("aruco cam ", id, " - HTTP error: ", response_code);
        return false;
    }
}

pid_t startPythonProgram(const std::string &scriptPath) {
    pid_t pid = fork();

    if (pid == -1) {
        LOG_ERROR("startPythonProgram - Failed to fork process");
        return -1;
    } else if (pid == 0) {
        // Child process: Execute the Python program
        execlp("python3", "python3", scriptPath.c_str(), nullptr);
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
        LOG_INFO("Process with PID ", pid, " terminated");
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    if (response) {
        response->append(static_cast<char*>(contents), totalSize);
    }
    return totalSize;
}