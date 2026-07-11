CXX = g++
CXXFLAGS = -std=c++20 -g -Iinclude

SRC = $(wildcard src/*.cpp)

OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)