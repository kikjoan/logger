CXX = gcc
CXX_FLAGS = -std=c++17 -Wall -Wextra -Werror -g
CXX_LIB_FLAGS = -lstdc++
TEST_FLAGS = -lgtest -lgmock

LIB_NAME = lib/liblogger.so
LIB_SRCS = src/logger/logger.cpp
LIB_HEADERS = src/logger/logger.h
LIB_OBJS = $(LIB_SRCS:.cpp=.o)
LIB_SRCS_TESTS = src/tests/logger_test.cpp
LIB_TESTS_NAME = src/tests/logger_test

LDFLAGS = -L${CURDIR}/lib/ -llogger -Wl,-rpath=${CURDIR}/lib/


APP_NAME = logger_app
APP_SRCS = src/app/logger_app.cpp src/app/main.cpp
APP_UTILS = src/app/utils.h
APP_OBJS = $(APP_SRCS:.cpp=.o)
APP_TESTS =

make: app

lib : $(LIB_NAME)

app : $(LIB_NAME) $(APP_NAME)

test : lib
	$(CXX) $(CXX_FLAGS) $(LIB_SRCS_TESTS) $(LIB_OBJS) $(CXX_LIB_FLAGS) -o $(LIB_TESTS_NAME) $(TEST_FLAGS)
	./src/tests/logger_test

$(APP_NAME) : $(APP_OBJS)
	$(CXX) $(CXX_FLAGS) $(APP_OBJS) -o $@ $(CXX_LIB_FLAGS) -llogger $(LDFLAGS)


$(LIB_NAME): $(LIB_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -shared $(LIB_OBJS) -o $@ $(CXX_LIB_FLAGS)


%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -fPIC -c $< -o $@

clean:
	rm -f $(LIB_OBJS) $(LIB_NAME)
	rm -f $(APP_OBJS) $(APP_NAME)
	rm -f $(LIB_TESTS_NAME)
	rm -f logs/*