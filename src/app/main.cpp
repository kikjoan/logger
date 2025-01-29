#include <iostream>

#include "logger_app.h"

using namespace logger;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_file> <default_log_level{INFO, WARN, ERROR}>" << std::endl;
        return 1;
    }
    if (argc == 2)
        LoggerApp(argv[1]).input();
    else
        LoggerApp(argv[1], argv[2]).input();

    return 0;
}
