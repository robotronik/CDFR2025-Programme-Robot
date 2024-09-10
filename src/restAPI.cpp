#include "restAPI.hpp"
#include "logger.hpp"
#include "main.hpp" //for static variables
#include "tableState.hpp"

// Crow app
static crow::SimpleApp app;

#define API_PORT 8080

using json = nlohmann::json;

void StartAPIServer(){
    LOG_INFO("Starting API Server on port ", API_PORT);

    // Define a simple route for the root endpoint
    CROW_ROUTE(app, "/")
    ([](){
        return "Welcome to the ROBOTRONIK Restful API!";
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
        return crow::response(response.dump(4));
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

    // Set the port and run the app
    app.loglevel(crow::LogLevel::Warning);
    app.port(API_PORT).multithreaded().run();
}

void StopAPIServer(){
    LOG_INFO("Stopped API Server");
    app.stop();
}