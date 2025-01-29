#ifndef LOGGER_APP_H
#define LOGGER_APP_H
#include "../logger/logger.h"
#include "utils.h"

namespace logger {
    class LoggerApp {
    public:
        LoggerApp(const std::string &file_name, const std::string &level);

        LoggerApp(const std::string &file_name);

        static Logger ConstructLogger(const std::string &file_name, const std::string &level);

        void WriteLog(const std::string &message, LogLevel level);

        void WriteLog(const std::string &message);

        void SetDefaultLogLevel(LogLevel level);

        void input();

    private:
        Logger logger;
    };
}

#endif //LOGGER_APP_H
