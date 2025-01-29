#include "logger_app.h"

#include <cinttypes>
#include <iostream>
#include <thread>
#include "../utils.h"
#include "../thread_safe_queue/thread_safe_queue.h"
using namespace logger;

LoggerApp::LoggerApp(const std::string &file_name, const std::string &level) : logger(file_name,
                                                                                   Utils::string_to_log_level(level)),
                                                                               running(true) {
    workerThread = std::thread(&LoggerApp::worker, this);
}

LoggerApp::LoggerApp(const std::string &file_name) : logger(file_name, LOG_LEVEL_INFO), running(true) {
    workerThread = std::thread(&LoggerApp::worker, this);
}

LoggerApp::~LoggerApp() {
    running = false;
    if (workerThread.joinable()) {
        taskQueue.push(LogTask{"", NO_FIND});
        workerThread.join();
    }
}


void LoggerApp::SetDefaultLogLevel(LogLevel level) {
    logger.SetDefaultLogLevel(level);
}

void LoggerApp::run() {
    std::string message;
    LogLevel level_from_message = logger.GetLevel();

    std::cout << "- To add a record level at the end, add {ERR, WARN, INFO} \n" <<
            "- To change the default log level, enter {edit} and a new log level {ERR, WARN, INFO}\n"
            << "- Exit {exit, q}\n" << std::endl;

    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if (message == "exit" || message == "q") {
            break;
        }

        level_from_message = Utils::find_log_level_from_message(message);

        if (message == "edit") {
            std::cout << "{ERR, WARN, INFO}" << std::endl;
            std::getline(std::cin, message);
            level_from_message = Utils::find_log_level_from_message(message);
            if (level_from_message == NO_FIND)
                std::cout << "An incorrect new log level has been entered. Expected {ERR, INFO, WARN}" << std::endl;
            else {
                logger.SetDefaultLogLevel(level_from_message);
                std::cout << "DONE" << std::endl;
            }
            continue;
        }

        if (level_from_message != NO_FIND) {
            switch (level_from_message) {
                case LOG_LEVEL_INFO:
                case LOG_LEVEL_WARN:
                    message = message.substr(0, message.length() - 4);
                    break;
                case LOG_LEVEL_ERR:
                    message = message.substr(0, message.length() - 3);
                    break;
            }
            taskQueue.push(LogTask{message, level_from_message});
        } else taskQueue.push(LogTask{message, logger.GetLevel()});
    };
}

void LoggerApp::worker() {
    while (running) {
        LogTask task = taskQueue.pop();
        if (!running && task.level == NO_FIND) break;
        logger.WriteLog(task.message, task.level);
    }
}
