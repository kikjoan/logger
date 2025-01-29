//
// Created by alex on 1/29/25.
//

#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace logger {
    template<typename T>
    class ThreadSafeQueue {
    public:
        void push(const T &value) {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(value);
            condition.notify_one();
        }

        T pop() {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this] { return !queue.empty(); });
            T value = queue.front();
            queue.pop();
            return value;
        }

        bool isEmpty() {
            std::lock_guard<std::mutex> lock(mutex);
            return queue.empty();
        }

        ~ThreadSafeQueue() {
            std::lock_guard<std::mutex> lock(mutex);
            condition.notify_all();
        }

    private:
        std::queue<T> queue;
        std::mutex mutex;
        std::condition_variable condition;
    };
}


#endif //THREAD_SAFE_QUEUE_H
