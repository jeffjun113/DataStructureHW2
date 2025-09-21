# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Find all .cpp files in the folder
SRCS := $(wildcard *.cpp)

# Create a list of executables (strip .cpp)
EXES := $(SRCS:.cpp=)

# Default target: build all executables
all: $(EXES)

# Rule to build each executable from its .cpp
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Clean up all generated files
clean:
	rm -f $(EXES)
.PHONY: all clean