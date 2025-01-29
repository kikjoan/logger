#include "logger.h"
#include "../app/utils.h"
#include <ios>
#include <iostream>
using namespace logger;
std::unordered_map<std::string, std::shared_ptr<std::mutex> > Logger::file_mutexes;
std::mutex Logger::map_mutex;

// The file was created in the logs directory in the root of the project.
Logger::Logger(const std::string &file_name, LogLevel level) : level(level), file_name(file_name) {
    if (!file_name.empty()) {
        std::lock_guard<std::mutex> lock(GetMutex(file_name));
        log_file.open("logs/" + file_name + ".log", std::ios::app);
        if (!log_file.is_open())
            std::cerr << "Error opening log file!" << std::endl;
    }
};

Logger::~Logger() {
    if (log_file.is_open()) log_file.close();
}

LogLevel Logger::SetDefaultLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(GetMutex(file_name));
    this->level = level;
    return this->level;
}

void Logger::WriteLog(const std::string &message, LogLevel level) {
    std::lock_guard<std::mutex> lock(GetMutex(file_name));
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

std::mutex &Logger::GetMutex(const std::string &file_name) {
    std::lock_guard<std::mutex> lock(map_mutex);
    if (file_mutexes.find(file_name) == file_mutexes.end()) {
        file_mutexes[file_name] = std::make_shared<std::mutex>();
    }
    return *file_mutexes[file_name];
}
