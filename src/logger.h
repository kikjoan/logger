#ifndef LOGGER_H
#define LOGGER_H
#include <cstdio>
#include <string>
#include <fstream>

namespace logger {
    enum LogLevel {
        LOG_LEVEL_INFO = 0,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
    };

    class Logger {
    public:
        Logger(const std::string &file, LogLevel level);

        ~Logger();

        void SetDefaultLogLevel(LogLevel level);

        void WriteLog(const std::string &message, LogLevel level);

        void WriteLog(const std::string &message);

        static std::string LogLevelToSting(LogLevel level);

    private:
        LogLevel level;
        std::ofstream log_file;
    };
};


#endif //LOGGER_H
