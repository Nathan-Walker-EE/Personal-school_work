# Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
CFLAGS = -Wall -Wextra -O2

# Source files and target executable
SRC = main.cpp problem_functions.cpp time_domain.cpp freq_func.cpp fft842.c 
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)
TARGET = fft

# Default target
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)
