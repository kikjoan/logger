#ifndef LOGGER_APP_H
#define LOGGER_APP_H
#include <thread>

#include "../../logger/logger.h"
#include "../utils.h"
#include "../thread_safe_queue/thread_safe_queue.h"

namespace logger {
    struct LogTask {
        std::string message;
        LogLevel level;
    };

    class LoggerApp {
    public:
        LoggerApp(const std::string &file_name, const std::string &level);

        explicit LoggerApp(const std::string &file_name);

        ~LoggerApp();

        void SetDefaultLogLevel(LogLevel level);

        void run();

    private:
        Logger logger;
        ThreadSafeQueue<LogTask> taskQueue;
        std::thread workerThread;
        bool running;

        void worker();
    };
}

#endif //LOGGER_APP_H
