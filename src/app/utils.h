
#ifndef UTILS_H
#define UTILS_H
#include <map>
#include "../logger/logger.h"

namespace logger {
    inline struct str_levels {
        std::map<std::string, LogLevel> levels = {
            {
                {"ERROR", LOG_LEVEL_ERROR}, {"error", LOG_LEVEL_ERROR}, {"WARN", LOG_LEVEL_WARN},
                {"warn", LOG_LEVEL_WARN},
                {"INFO", LOG_LEVEL_INFO}, {"info", LOG_LEVEL_INFO}
            }
        };
    } str_levels;

    class Utils {
    public:
        static LogLevel string_to_log_level(const std::string &level) {
            auto i = str_levels.levels.find(level);
            if (i == str_levels.levels.end())return LogLevel(0);
            else
                return i->second;
        }

        static std::string log_level_to_string(LogLevel level) {
            switch (level) {
                case LOG_LEVEL_INFO: return "LOG_LEVEL_INFO";
                case LOG_LEVEL_WARN: return "LOG_LEVEL_WARN";
                case LOG_LEVEL_ERROR: return "LOG_LEVEL_ERROR";
                default: return "NO_FIND";
            }
        }

        static LogLevel find_log_level_from_message(const std::string &message) {
            if (message.length() < 4) return NO_FIND;
            std::string last_five = message.substr(message.length() - 4, message.length() - 1);
            for (auto &level: str_levels.levels) {
                if (level.first.find(last_five)) return level.second;
            }
            return NO_FIND;
        }
    };
};


#endif //UTILS_H
