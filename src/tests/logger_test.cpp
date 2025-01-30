
#include "../logger/logger.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace logger;

std::string ReadFileContent(const std::string &filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

class LoggerTest : public ::testing::Test {
protected:
    std::string test_log_file = "test_log";
    std::string test_log_file_path = "logs/" + test_log_file + ".log";
    Logger *logger;

    void SetUp() override {
        logger = new Logger(test_log_file, LOG_LEVEL_INFO);
    }

    void TearDown() override {
        delete logger;
        if (fs::exists(test_log_file + ".log")) {
            fs::remove(test_log_file + ".log");
        }
    }
};

TEST_F(LoggerTest, LogFileCreation) {
    logger->WriteLog(test_log_file);
    EXPECT_TRUE(fs::exists(test_log_file_path));
}

TEST_F(LoggerTest, WriteInfoLog) {
    logger->WriteLog("Test message", LOG_LEVEL_INFO);
    std::string content = ReadFileContent(test_log_file_path);
    EXPECT_NE(content.find("Test message"), std::string::npos);
    EXPECT_NE(content.find("LOG_LEVEL_INFO"), std::string::npos);
}

TEST_F(LoggerTest, LogLevelFilter) {
    logger->SetDefaultLogLevel(LOG_LEVEL_WARN);
    logger->WriteLog("Ignored message", LOG_LEVEL_INFO);
    logger->WriteLog("Important message", LOG_LEVEL_WARN);

    std::string content = ReadFileContent(test_log_file_path);
    EXPECT_EQ(content.find("Ignored message"), std::string::npos);
    EXPECT_NE(content.find("Important message"), std::string::npos);
}

TEST_F(LoggerTest, WriteDefaultLevelLog) {
    logger->WriteLog("Default level message");
    std::string content = ReadFileContent(test_log_file_path);
    EXPECT_NE(content.find("Default level message"), std::string::npos);
    EXPECT_NE(content.find("LOG_LEVEL_INFO"), std::string::npos);

    logger->SetDefaultLogLevel(LOG_LEVEL_ERR);
    logger->WriteLog("INFO MESSAGE", LOG_LEVEL_INFO);

    EXPECT_EQ(content.find("INFO MESSAGE"), std::string::npos);
}

TEST_F(LoggerTest, ErrorOpeningFile) {
    Logger invalid_logger("", LOG_LEVEL_INFO);
    std::string invalid_log_file = ".log";
    EXPECT_FALSE(fs::exists(invalid_log_file));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
