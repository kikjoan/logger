CXX = gcc
CXX_FLAGS = -std=c++17 -Wall -Wextra -Werror -g
CXX_LIB_FLAGS = -lstdc++
TEST_FLAGS = -lgtest -lgmock

LIB_NAME = lib/liblogger.so
LIB_SRCS = src/logger/logger.cpp
LIB_OBJS = $(LIB_SRCS:.cpp=.o)
LIB_TESTS = src/test/logger_test.cpp

APP_NAME = src/app/logger_app
APP_SRCS = src/app/logger_app.cpp src/app/main.cpp
APP_OBJS = $(APP_SRCS:.cpp=.o)
APP_TESTS =


all: $(LIB_NAME) $(APP_NAME)

$(APP_NAME) : $(APP_OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $(APP_OBJS) $(CXX_LIB_FLAGS)

$(LIB_NAME) : $(LIB_OBJS)
	$(CXX) $(CXX_FLAGS) -fPIC -shared -o $@ $(LIB_OBJS) $(CXX_LIB_FLAGS)

$(LIB_TESTS):
	$(CXX) $(CXX_FLAGS) $(LIB_TESTS) $(LIB_SRCS) $(CXX_LIB_FLAGS) $(TEST_FLAGS)

clean:
	rm -f $(LIB_OBJS) $(LIB_NAME)
	rm -f $(APP_OBJS) $(APP_NAME)