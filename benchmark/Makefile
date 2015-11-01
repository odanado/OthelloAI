CXXFLAGS = -Wall -Wextra -std=c++11
OPTFLAGS = -O3 -march=native -fno-rtti -fno-stack-protector -DNDEBUG

SOURCES = $(wildcard *.cpp)
OBJS  = $(addprefix ./, $(SOURCES:.cpp=.o))

TARGET = benchmark.out

benchmark:$(OBJS)
		$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(OBJS) -o $(TARGET)
%.o: %.cpp
		$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ -c $<

clean:
	- rm -rf $(OBJS) $(TARGET)
