#include "logger.hpp"

bool testLogger(){
	Logger::getInstance().log(LogLevel::DEBUG, "This is a debug message!");
	LOG_INIT();
	LOG_DEBUG("This is a debug message!");
	LOG_INFO("This is an info message!");
	LOG_GREEN_INFO("This is a green info message!");
	LOG_WARNING("This is a warning message!");
	LOG_ERROR("This is an error message!");
	LOG_SCOPE("SCOPE");
	LOG_STATE("STATE");
	return true;
}