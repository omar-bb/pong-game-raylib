
# directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := obj

# the program name
TARGET := pong

# cpp compiler and flags
CXX := clang++
PKG_CONFIG_OUTPUT := $(shell pkg-config --cflags --libs raylib)
CXXFLAGS := -std=c++17

# sources, objs and executable
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
OUTPUT := $(BUILD_DIR)/$(TARGET)

all: dirs $(OUTPUT)

dirs:
	@echo "Creating the build and obj directories..."
	mkdir -p $(BUILD_DIR) $(OBJ_DIR)

$(OUTPUT): $(OBJS)
	@echo "Linking..."
	$(CXX) $(PKG_CONFIG_OUTPUT) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling source files..."
	$(CXX) $(PKG_CONFIG_OUTPUT) $(CXXFLAGS) -c $< -o $@

run: $(OUTPUT)
	./$(OUTPUT)

clean:
	rm -rf $(OUTPUT) $(OBJ_DIR)

.PHONY: all clean run

