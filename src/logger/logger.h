#ifndef LOGGER_H
#define LOGGER_H
#include <cstdio>
#include <string>
#include <fstream>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace logger {
    // перечисление уровней логирования
    enum LogLevel {
        NO_FIND = 0,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERR
    };

    // глобальный мьютекс для предотвращения одновременного доступа к записи в файл из разных потоков
    static std::mutex logger_mutex;

    class Logger {
    public:
        // конструктор принимает файл, в который нужно записывать и уровень логирования по умолчанию
        Logger(const std::string &file_name, LogLevel level);

        ~Logger();

        // устанавливает уровень логирования по умолчанию
        LogLevel SetDefaultLogLevel(LogLevel level);

        // принимает и записывает сообщение с указанным уровнем логирования
        void WriteLog(const std::string &message, LogLevel level);

        // приниает и записывает сообщение с уровнем логирования по умолчанию
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
