#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <curl/curl.h>
#include "ArucoCam.hpp"
#include "logger.hpp"

#define PORT_OFFSET 5000

ArucoCam::ArucoCam(int cam_number) {
    pid = startPythonProgram("detect_aruco.py api-port " + std::to_string(PORT_OFFSET + cam_number) + " cam " + std::to_string(cam_number));
    if (pid == -1) {
        LOG_ERROR("Failed to start ArucoCam ", cam_number);
    }
}
ArucoCam::~ArucoCam(){
    stopPythonProgram(pid);
}
bool ArucoCam::getPos(){
    // Calls /position rest api endpoint of the ArucoCam API
    // Returns true if the call was successful, false otherwise
}
// Callback to write the response data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    if (response) {
        response->append(static_cast<char*>(contents), totalSize);
    }
    return totalSize;
}

bool callPositionAPI() {
    const std::string url = "http://localhost:5000/position";
    std::string responseBody;

    // Initialize a CURL handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
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
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
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
            nlohmann::json jsonResponse = nlohmann::json::parse(responseBody);
            std::cout << "API Response: " << jsonResponse.dump(4) << std::endl;
            return true;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            return false;
        }
    } else {
        std::cerr << "HTTP error: " << response_code << std::endl;
        return false;
    }
}

pid_t startPythonProgram(const std::string &scriptPath) {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return -1;
    } else if (pid == 0) {
        // Child process: Execute the Python program
        execlp("python3", "python3", scriptPath.c_str(), nullptr);
        std::cerr << "Failed to execute Python script" << std::endl;
        _exit(1); // Ensure child process exits
    }

    // Parent process: Return child PID
    return pid;
}

void stopPythonProgram(pid_t pid) {
    if (pid <= 0) {
        std::cerr << "Invalid PID" << std::endl;
        return;
    }

    if (kill(pid, SIGTERM) == -1) {
        std::cerr << "Failed to terminate process with PID: " << pid << std::endl;
    } else {
        std::cout << "Process with PID " << pid << " terminated" << std::endl;
    }
}