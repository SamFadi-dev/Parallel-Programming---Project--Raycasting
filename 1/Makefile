CXX := g++

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
EXECUTABLE := raycasting

CXXFLAGS := -std=c++11 -I$(INCLUDE_DIR) -Wall -W -O3 -fopenmp

LDFLAGS := -lX11 -fopenmp

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(EXECUTABLE)

.PHONY: all clean
