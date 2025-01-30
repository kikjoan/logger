#ifndef LOGGER_APP_H
#define LOGGER_APP_H
#include <thread>
#include <mutex>
#include "../../logger/logger.h"
#include "../utils.h"
#include "../thread_safe_queue/thread_safe_queue.h"

namespace logger {
    // структура, которая формируется в run и отправляется в очередь
    struct LogTask {
        std::string message;
        LogLevel level;
    };

    // для всех потоков доступ к очереди должен быть потокобезопасным
    static ThreadSafeQueue<LogTask> taskQueue;

    class LoggerApp {
    public:
        /*
         *  Конструкторы и деструктор
         *  Первый конструктор принимает имя файла и уровень логирования
         */
        LoggerApp(const std::string &file_name, const std::string &level);

        explicit LoggerApp(const std::string &file_name);

        ~LoggerApp();

        /*
         *  Методы
         *  run - запускает приложение, обрабатывает ввод пользователя и отправляет сообщения в очередь
         *  isRunning - возвращает true, если приложение запущено
         */
        void run();

        bool isRunning() const { return running; }

    private:
        Logger logger;
        std::thread workerThread;
        bool running;
        void SetDefaultLogLevel(LogLevel level);
        void worker();
    };
}

#endif //LOGGER_APP_H
