#ifndef LOGGER_H
#define LOGGER_H
#include <cstdio>
#include <string>
#include <fstream>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace logger {
    enum LogLevel {
        NO_FIND = 0,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERR
    };

    class Logger {
    public:
        Logger(const std::string &file_name, LogLevel level);

        ~Logger();

        LogLevel SetDefaultLogLevel(LogLevel level);

        void WriteLog(const std::string &message, LogLevel level);

        void WriteLog(const std::string &message);

        [[nodiscard]] LogLevel GetLevel() const {
            return level;
        }

    private:
        LogLevel level;
        std::ofstream log_file;
        std::string file_name;
    };
};


#endif //LOGGER_H
