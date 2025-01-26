CXX = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -g -fPIC
LIBFLAGS = -lstdc++
TESTFLAGS = -lgtest -lgmock
LIB_NAME = lib/liblogger.so
SRCS = src/logger.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(LIB_NAME)

$(LIB_NAME) : $(OBJS)
	$(CXX) -shared -o $@ $(OBJS) $(LIBFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

test:
	$(CC) $(CFLAGS) src/test/logger_test.cpp $(SRCS) $(LIBFLAGS) $(TESTFLAGS)

clean:
	rm -f $(OBJS) $(LIB_NAME)

