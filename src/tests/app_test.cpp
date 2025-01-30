#include <gtest/gtest.h>
#include "../app/logger_app/logger_app.h"
#include <filesystem>
using namespace logger;
namespace fs = std::filesystem;

std::string ReadFileContent(const std::string &filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

class LoggerAppTest : public ::testing::Test {
protected:
    std::string test_log_file = "test_log";
    std::string test_log_file_path = "logs/" + test_log_file + ".log";


    void SetUp() override {
        if (fs::exists(test_log_file_path)) {
            fs::remove(test_log_file_path);
        }
    }
};

TEST_F(LoggerAppTest, WriteInfoLog) {
    LoggerApp app(test_log_file, "INFO");
    std::istringstream input("Test message\nq\n");
    std::cin.rdbuf(input.rdbuf());

    app.run();

    while (app.isRunning());

    std::string logContent = ReadFileContent(test_log_file_path);

    EXPECT_NE(logContent.find("Test message"), std::string::npos);
    EXPECT_NE(logContent.find("LOG_LEVEL_INFO"), std::string::npos);
}

TEST_F(LoggerAppTest, SetDefaultLogLevel) {
    LoggerApp app(test_log_file, "INFO");
    std::istringstream input("Test message ERR\nTest message WARN\nq\n");
    std::cin.rdbuf(input.rdbuf());

    app.run();

    while (app.isRunning());

    std::string logContent = ReadFileContent(test_log_file_path);


    EXPECT_NE(logContent.find("Test message"), std::string::npos);
    EXPECT_NE(logContent.find("LOG_LEVEL_ERROR"), std::string::npos);
    EXPECT_NE(logContent.find("LOG_LEVEL_WARN"), std::string::npos);
}

TEST_F(LoggerAppTest, ChangeDefaultLogLevel) {
    LoggerApp app(test_log_file, "INFO");
    std::istringstream input("edit\nERR\nTest message WARN\nTest message\nq\n");
    std::cin.rdbuf(input.rdbuf());

    app.run();
    while (app.isRunning());

    std::string logContent = ReadFileContent(test_log_file_path);


    EXPECT_NE(logContent.find("Test message"), std::string::npos);
    EXPECT_EQ(logContent.find("LOG_LEVEL_WARN"), std::string::npos);
    EXPECT_NE(logContent.find("LOG_LEVEL_ERROR"), std::string::npos);
}

// тест не всегда проходит. cin - глобальный объект. нужно изменить подход, добавлять задачи через AddTask
TEST_F(LoggerAppTest, Multithreading) {
    LoggerApp app1(test_log_file, "INFO");
    LoggerApp app2(test_log_file, "ERR");

    std::thread t1([&app1]() {
        std::istringstream input(
            "Test message 1\nTest message 1\nTest message 1\nTest message 1\nTest message 1\nq\n");
        std::streambuf *originalCin = std::cin.rdbuf(input.rdbuf());
        app1.run();
        std::cin.rdbuf(originalCin);
    });

    std::thread t2([&app2]() {
        std::istringstream input(
            "Test message 2\nTest message 2\nTest message 2\nTest message 2\nTest message 2\nq\n");
        std::streambuf *originalCin = std::cin.rdbuf(input.rdbuf());
        app2.run();
        std::cin.rdbuf(originalCin);
    });

    t1.join();
    t2.join();

    while (app1.isRunning() || app2.isRunning());

    std::string logContent = ReadFileContent(test_log_file_path);

    EXPECT_NE(logContent.find("Test message 1"), std::string::npos);
    EXPECT_NE(logContent.find("Test message 2"), std::string::npos);
    EXPECT_NE(logContent.find("LOG_LEVEL_INFO"), std::string::npos);
    EXPECT_NE(logContent.find("LOG_LEVEL_ERROR"), std::string::npos);
}

TEST(ThreadSafeQueueTest, ThreadSafety) {
    ThreadSafeQueue<int> queue;

    std::thread producer([&queue]() {
        for (int i = 0; i < 100; ++i) {
            queue.push(i);
        }
    });

    std::thread consumer([&queue]() {
        for (int i = 0; i < 100; ++i) {
            int value = queue.pop();
            EXPECT_EQ(value, i);
        }
    });

    producer.join();
    consumer.join();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
