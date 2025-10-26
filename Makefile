# Compiler and flags
CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Source files
SRC := main.cpp $(wildcard src/*.cpp)

# Object files
OBJ := $(SRC:.cpp=.o)

# Target executable
TARGET := main.exe

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# Compile .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean