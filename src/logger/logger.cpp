#include "logger.h"
#include "../app/utils.h"
#include <ios>
#include <iostream>
using namespace logger;

// The file was created in the logs directory in the root of the project.
Logger::Logger(const std::string &file_name, LogLevel level) : level(level), file_name(file_name) {
    if (!file_name.empty()) {
        log_file.open("logs/" + file_name + ".log", std::ios::app);
        if (!log_file.is_open())
            std::cerr << "Error opening log file!" << std::endl;
    }
};

Logger::~Logger() {
    if (log_file.is_open()) log_file.close();
}

LogLevel Logger::SetDefaultLogLevel(LogLevel level) {

    this->level = level;
    return this->level;
}

void Logger::WriteLog(const std::string &message, LogLevel level) {
    if (level >= this->level) {
        if (log_file.is_open()) {
            time_t time = ::time(nullptr);
            log_file << message << " | " << Utils::log_level_to_string(level) << " | " <<
                    asctime(localtime(&time)) << std::endl;;
        } else {
            std::cerr << "Error writing to log file!" << std::endl;
        }
    }
}

void Logger::WriteLog(const std::string &message) {
    WriteLog(message, level);
}
