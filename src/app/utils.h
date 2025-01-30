
#ifndef UTILS_H
#define UTILS_H
#include <map>
#include "../logger/logger.h"

namespace logger {
    // Структура для хранения соответствия строковых значений уровней логирования и их числовых значений
    inline struct str_levels {
        std::map<std::string, LogLevel> levels = {
            {
                {"ERR", LOG_LEVEL_ERR}, {"err", LOG_LEVEL_ERR}, {"WARN", LOG_LEVEL_WARN},
                {"warn", LOG_LEVEL_WARN},
                {"INFO", LOG_LEVEL_INFO}, {"info", LOG_LEVEL_INFO}
            }
        };
    } str_levels;

    class Utils {
    public:
        // Функция для преобразования строкового значения уровня логирования в числовое
        static LogLevel string_to_log_level(const std::string &level) {
            auto i = str_levels.levels.find(level);
            if (i == str_levels.levels.end()) return LogLevel(1);
            else
                return i->second;
        }

        // Функция для преобразования числового значения уровня логирования в строковое
        static std::string log_level_to_string(LogLevel level) {
            switch (level) {
                case LOG_LEVEL_INFO: return "LOG_LEVEL_INFO";
                case LOG_LEVEL_WARN: return "LOG_LEVEL_WARN";
                case LOG_LEVEL_ERR: return "LOG_LEVEL_ERROR";
                default: return "NO_FIND";
            }
        }

        /*
         * Функция для поиска уровня логирования в сообщении
         * last_four - последние 4 символа в сообщении для обработки WARN и INFO
         * last_three - последние 3 символа в сообщении для обработки ERR
         */
        static LogLevel find_log_level_from_message(const std::string &message) {
            if (message.length() < 3) return NO_FIND;
            std::string last_four;
            std::string last_three;
            if (message.length() >= 4)
                last_four = message.substr(message.length() - 4, message.length());
            last_three = message.substr(message.length() - 3, message.length());
            for (auto &level: str_levels.levels) {
                if (!last_four.empty() && level.first.find(last_four) != std::string::npos) return level.second;
                if (level.first.find(last_three) != std::string::npos) return level.second;
            }
            return NO_FIND;
        }
    };
};


#endif //UTILS_H

