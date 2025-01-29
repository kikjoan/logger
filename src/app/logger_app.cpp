#include "logger_app.h"

#include <cinttypes>
#include <iostream>
#include <thread>
#include "utils.h"
using namespace logger;

LoggerApp::LoggerApp(const std::string &file_name, const std::string &level) : logger(
    ConstructLogger(file_name, level)) {
}

LoggerApp::LoggerApp(const std::string &file_name) : logger(file_name, LOG_LEVEL_INFO) {
    std::cout << logger.GetLevel() << std::endl;
}

Logger LoggerApp::ConstructLogger(const std::string &file_name, const std::string &level) {
    int lvl = Utils::string_to_log_level(level);
    if (lvl == 0) {
        std::cerr << "ERROR INPUT <default_log_level{INFO, WARN, ERROR}>" << std::endl;
        exit(1);
    } else return Logger(file_name, LogLevel(lvl));
}

void LoggerApp::WriteLog(const std::string &message, logger::LogLevel level) {
    std::thread write([&]() { logger.WriteLog(message, level); });
    write.detach();
}

void LoggerApp::WriteLog(const std::string &message) {
    WriteLog(message, logger.GetLevel());
}

void LoggerApp::SetDefaultLogLevel(LogLevel level) {
    logger.SetDefaultLogLevel(level);
}

void LoggerApp::input() {
    std::string message;
    LogLevel level = logger.GetLevel();

    std::cout << "- To add a record level at the end, add {ERROR, WARN, INFO} \n" <<
            "- To change the default log level, enter {edit} and a new log level {ERROR, WARN, INFO}" << std::endl;

    while (true) {
        std::cin >> message;

        if (message == "STOP" || message == "q") break;

        level = Utils::find_log_level_from_message(message);

        if (message == "edit") {
            std::cout << "{ERROR, WARN, INFO}" << std::endl;
            std::cin >> message;
            level = Utils::find_log_level_from_message(message);
            if (level == NO_FIND)
                std::cout << "An incorrect new log level has been entered. Expected {ERROR, INFO, WARN}" << std::endl;
            else {
                logger.SetDefaultLogLevel(level);
                std::cout << "DONE" << std::endl;
            }
            continue;
        }

        if (level != NO_FIND) {
            switch (level) {
                case LOG_LEVEL_INFO:
                case LOG_LEVEL_WARN:
                    message = message.substr(0, message.length() - 3);
                    break;
                case LOG_LEVEL_ERROR:
                    message = message.substr(0, message.length() - 4);
                    break;
                case NO_FIND:
                    level = logger.GetLevel();
                    break;
            }
            WriteLog(message, level);
        } else WriteLog(message);
    }
}
