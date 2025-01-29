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
        LOG_LEVEL_ERROR
    };

    class Logger {
    public:
        Logger(const std::string &file_name, LogLevel level);

        ~Logger();

        LogLevel SetDefaultLogLevel(LogLevel level);

        void WriteLog(const std::string &message, LogLevel level);

        void WriteLog(const std::string &message);

        static std::mutex &GetMutex(const std::string &file_name);

        [[nodiscard]] LogLevel GetLevel() const {
            return level;
        }

        std::lock_guard<std::mutex> LockMutex();

    private:
        LogLevel level;
        std::ofstream log_file;
        std::string file_name;


        static std::unordered_map<std::string, std::shared_ptr<std::mutex> > file_mutexes;
        static std::mutex map_mutex;
    };
};


#endif //LOGGER_H
