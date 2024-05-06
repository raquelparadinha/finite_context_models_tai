CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
EXEC = $(BIN_DIR)/markov_model_classifier

all: $(EXEC)

$(EXEC): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(EXEC)
	$(EXEC) $(ARGS)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
