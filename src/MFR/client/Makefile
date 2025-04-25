CXX = g++
CXXFLAGS = -std=c++11 -Wall -I../common

TARGET = SIM
SRCS = SIM.cpp defineAlgorithm.cpp loaderINI.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
