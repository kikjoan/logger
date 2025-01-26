#include "logger.h"

#include <ios>
#include <iostream>
using namespace logger;

// The file was created in the logs directory in the root of the project.
Logger::Logger(const std::string &file, LogLevel level) : level(level) {
    if (!file.empty()) {
        log_file.open("logs/" + file + ".log", std::ios::app);
        if (!log_file.is_open())
            std::cerr << "Error opening log file!" << std::endl;
    }
};

Logger::~Logger() {
    if (log_file.is_open()) log_file.close();
}

void Logger::SetDefaultLogLevel(LogLevel level) {
    this->level = level;
}

void Logger::WriteLog(const std::string &message, LogLevel level) {
    if (level >= this->level) {
        if (log_file.is_open()) {
            std::time_t time = std::time(nullptr);
            log_file << message << " | " << LogLevelToSting(level) << " | " <<
                    std::asctime(std::localtime(&time)) << std::endl;;
        } else {
            std::cerr << "Error writing to log file!" << std::endl;
        }
    }
}

void Logger::WriteLog(const std::string &message) {
    WriteLog(message, level);
}

std::string Logger::LogLevelToSting(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_INFO: return "LOG_LEVEL_INFO";
        case LOG_LEVEL_WARN: return "LOG_LEVEL_WARN";
        case LOG_LEVEL_ERROR: return "LOG_LEVEL_ERROR";
        default: return "NON_LEVEL_ERROR";
    }
}
