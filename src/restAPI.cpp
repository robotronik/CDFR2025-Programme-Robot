#include "restAPI.hpp"
#include <fstream>
#include <sstream>
#include <string>

#include "logger.hpp"
#include "main.hpp" //for static variables
#include "tableState.hpp"
#include "lidarAnalize.h" //for static variable


#define API_PORT 8080

using json = nlohmann::json;

crow::response readHtmlFile(const std::string& path);
std::string getContentType(const std::string& path);

crow::SimpleApp app;

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

    // Define a simple route for the lidar page
    CROW_ROUTE(app, "/logs")
    ([](){
        return readHtmlFile("html/logs.html");
    });

    // Define a simple route for the navbar
    CROW_ROUTE(app, "/navbar.html")
    ([](){
        return readHtmlFile("html/navbar.html");
    });

    // Define a simple route for robot page
    CROW_ROUTE(app, "/robot")
    ([](){
        return readHtmlFile("html/robot.html");
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
        response["pos"] = tableStatus.robot.pos;

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

    // Define a route for a simple GET request that returns all of the information
    CROW_ROUTE(app, "/get_global")
    ([](){
        json response;
        response["status"] = currentState;
        response["table"] = tableStatus;
        response["score"] = tableStatus.getScore();
        
        json limitedAbsoluteLidarData = json::array();

        // Add the first lidar_count elements to the new array
        for (int i = 0; i < lidar_count; ++i) {
            limitedAbsoluteLidarData.push_back({{"x", lidarData[i].x}, {"y", lidarData[i].y}});
        }
        response["lidar"] = limitedAbsoluteLidarData;

        return crow::response(response.dump());
    });

    // Define a route for an simple GET request that returns the lidar data
    CROW_ROUTE(app, "/get_lidar")
    ([](){
        json response;
        
        json limitedLidarData = json::array();

        // Add the first lidar_count elements to the new array
        for (int i = 0; i < lidar_count; ++i) {
            limitedLidarData.push_back(lidarData[i]);
        }
        response["data"] = limitedLidarData;
        response["detection_debug"] = beacon_detection;
        return crow::response(response.dump());
    });

    // Define a route for an simple GET request that returns the general data
    CROW_ROUTE(app, "/get_robot")
    ([](){
        json response;
        response["status"] = currentState;
        response["team"] = tableStatus.robot.colorTeam;
        response["score"] = tableStatus.getScore();
        response["time"] = _millis() - tableStatus.startTime;
        // TODO : Strategy number
        return crow::response(response.dump(4));
    });

    // Define a route for a POST request that accepts JSON data and responds with a message
    CROW_ROUTE(app, "/post_status").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        // Parse the incoming JSON
        LOG_INFO("Request body is " + req.body);
        auto req_data = json::parse(req.body);

        // Extract fields
        main_State_t req_state = req_data["state"];

        if (req_state < 0 || req_state > 6){
            // Denies the POST resquest
            json response;
            response["message"] = "Requested state out of borders!";

            // Return the response as JSON
            return crow::response(400, response.dump(4));
        }

        // Apply the post method
        nextState = req_state;

        // Create a response JSON
        json response;
        response["message"] = "Successfull";

        // Return the response as JSON
        return crow::response(response.dump(4));
    });

    // Define a route for a POST request that accepts a color as JSON data (0:None, 1:Blue, 2:Yellow)
    CROW_ROUTE(app, "/set_color").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto req_data = json::parse(req.body);
        colorTeam_t req_color = req_data["color"];

        if (req_color < 0 || req_color > 2){
            json response;
            response["message"] = "Invalid Request Color";
            return crow::response(400, response.dump(4));
        }
        if (currentState == RUN){
            json response;
            response["message"] = "Cannot change the color in the current state";
            return crow::response(400, response.dump(4));
        }

        tableStatus.robot.colorTeam = req_color;

        json response;
        response["message"] = "Successfull";
        return crow::response(response.dump(4));
    });

    // Define a route for a POST request that accepts a strategy to apply
    CROW_ROUTE(app, "/set_strat").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto req_data = json::parse(req.body);
        int req_strat = req_data["strat"];

        if (req_strat < 1 || req_strat > 4){
            json response;
            response["message"] = "Invalid Strategy Request";
            return crow::response(400, response.dump(4));
        }
        if (currentState == RUN){
            json response;
            response["message"] = "Cannot change the strategy in the current state";
            return crow::response(400, response.dump(4));
        }

        //TODO : Apply the strat

        json response;
        response["message"] = "Successfull";
        return crow::response(response.dump(4));
    });

    // Route for serving SVG and PNG files
    CROW_ROUTE(app, "/assets/<string>")
    .methods(crow::HTTPMethod::GET)([](const std::string& filename) {
        std::ifstream file("html/assets/" + filename, std::ios::binary);
        
        if (!file) {
            return crow::response(404, "File not found");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        crow::response res{buffer.str()};
        res.set_header("Cache-Control", "public, max-age=3600");
        res.set_header("Content-Type", getContentType(filename));
        return res;
    });

    // Route for serving css files
    CROW_ROUTE(app, "/css/<string>")
    .methods(crow::HTTPMethod::GET)([](const std::string& filename) {
        return readHtmlFile("html/css/" + filename);
    });
    // Route for serving js files
    CROW_ROUTE(app, "/js/<string>")
    .methods(crow::HTTPMethod::GET)([](const std::string& filename) {
        return readHtmlFile("html/js/" + filename);
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
        res.set_header("Cache-Control", "public, max-age=3600");
        res.set_header("Content-Type", "image/x-icon");
        return res;
    });


    // Set the port and run the app with 2 threads
    app.loglevel(crow::LogLevel::Warning);
    app.signal_clear().port(API_PORT).concurrency(2).run();
}

void StopAPIServer(){
    app.stop();
    LOG_INFO("Stopped API Server");
}

void TestAPIServer(){
    // Sets some variable to display them statically

    tableStatus.robot.pos.theta = 15;
    tableStatus.robot.pos.x = 100;
    tableStatus.robot.pos.y = 100;

    tableStatus.pos_opponent.x = 300;
    tableStatus.pos_opponent.y = 300;
}


// Function to read an HTML file and return its content as a crow::response
crow::response readHtmlFile(const std::string& path) {
    std::ifstream file(path);
    
    if (!file) {  // Check if file exists and is open
        return crow::response(404, "File not found");  // Return a 404 response if file is missing
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // Read the file into the buffer

    crow::response res{buffer.str()};
    res.set_header("Content-Type", getContentType(path));  // Set correct Content-Type header
    res.set_header("Cache-Control", "public, max-age=3600");
    return res;
}

// Helper function to determine Content-Type based on file extension
std::string getContentType(const std::string& path) {
    std::string extension = path.substr(path.find_last_of(".") + 1);
    if (extension == "css") {
        return "text/css";
    } else if (extension == "js") {
        return "application/javascript";
    } else if (extension == "svg") {
        return "image/svg+xml";
    } else if (extension == "png") {
        return "image/png";
    } else if (extension == "html"){
        return "text/html";
    } else {
        return "text/plain";  // Default to plain text
    }
}