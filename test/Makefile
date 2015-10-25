CXXFLAGS = -g -Wall -Wextra -pthread -std=c++11

SOURCES = $(wildcard *.cpp)
GTEST_SRC = $(wildcard ./gtest/*.cc)
OBJS  = $(addprefix ./, $(SOURCES:.cpp=.o))
OBJS  += $(addprefix , $(GTEST_SRC:.cc=.o))

TARGET = test.out

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	- rm -rf $(OBJS) $(TARGET)
