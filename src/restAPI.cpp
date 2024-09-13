#include "restAPI.hpp"
#include <fstream>
#include <sstream>
#include <string>

#include "logger.hpp"
#include "main.hpp" //for static variables
#include "tableState.hpp"

// Crow app
static crow::SimpleApp app;

#define API_PORT 8080

using json = nlohmann::json;

std::string readHtmlFile(const std::string& path);

void StartAPIServer(){
    LOG_INFO("Starting API Server on port ", API_PORT);

    // Define a simple route for the root endpoint
    CROW_ROUTE(app, "/")
    ([](){
        return readHtmlFile("html/index.html");
    });

    // Define a simple route for the display page
    CROW_ROUTE(app, "/display")
    ([](){
        return readHtmlFile("html/display.html");
    });

    // Define a simple route for the lidar page
    CROW_ROUTE(app, "/lidar")
    ([](){
        return readHtmlFile("html/lidar.html");
    });

    // Define a route for a simple GET request that returns a JSON response
    CROW_ROUTE(app, "/get_data")
    ([](){
        json response; //Create a nlohmann::json response
        response["message"] = "This is a GET request";
        response["value"] = 42;

        return crow::response(response.dump(4)); // 4 spaces for indentation
    });
    // Define a route for a simple GET request that returns the status
    CROW_ROUTE(app, "/get_status")
    ([](){
        json response;
        response["status"] = currentState;

        return crow::response(response.dump(4));
    });

    // Define a route for a simple GET request that returns the position
    CROW_ROUTE(app, "/get_pos")
    ([](){
        json response;
        response["x"] = x;
        response["y"] = y;
        response["teta"] = teta;

        return crow::response(response.dump(4));
    });

    // Define a route for a simple GET request that returns the table status
    CROW_ROUTE(app, "/get_table")
    ([](){
        json response;
        response["table"] = tableStatus;
        response["score"] = tableStatus.getScore();
        return crow::response(response.dump(4));
    });

    // Define a route for a simple GET request that returns the lidar data
    CROW_ROUTE(app, "/get_lidar")
    ([](){
        json response;
        response["data"] = lidarData;
        return crow::response(response.dump());
    });

    // Define a route for a POST request that accepts JSON data and responds with a message
    CROW_ROUTE(app, "/post_status").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        // Parse the incoming JSON
        LOG_INFO("Request body is " + req.body);
        auto req_data = json::parse(req.body);

        // Extract fields
        main_State_t req_state = req_data["state"];

        // Apply the post method
        currentState = req_state;

        // Create a response JSON
        json response;
        response["message"] = "Successfull";

        // Return the response as JSON
        return crow::response(response.dump(4));
    });


    // Route for serving SVG files
    CROW_ROUTE(app, "/assets/<string>")
    .methods(crow::HTTPMethod::GET)([](const std::string& filename) {
        std::ifstream file("html/" + filename, std::ios::binary);
        if (!file) {
            return crow::response(404, "File not found");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        crow::response res{buffer.str()};
        res.set_header("Content-Type", "image/svg+xml");
        return res;
    });

    // Route for serving the favicon ico files
    CROW_ROUTE(app, "/favicon.ico") ([](){
        std::ifstream file("html/favicon.ico", std::ios::binary);
        if (!file) {
            return crow::response(404, "File not found");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        crow::response res{buffer.str()};
        res.set_header("Content-Type", "image/x-icon");
        return res;
    });


    // Set the port and run the app
    app.loglevel(crow::LogLevel::Warning);
    app.port(API_PORT).multithreaded().run();
}

void StopAPIServer(){
    LOG_INFO("Stopped API Server");
    app.stop();
}


// Function to read an HTML file and return its content as a string
std::string readHtmlFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();  // Read the file into the buffer
    return buffer.str();
}