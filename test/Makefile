CXX = g++
INCLUDE = -I /usr/local/include
LIBS = -lgtest -lgtest_main
CXXFLAGS = -g -Wall -Wextra -pthread -std=c++11

OBJS = OthelloBoardTest.o
TARGET = test.o

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBS) $(OBJS) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

clean:
	rm $(OBJS) $(TARGET)
