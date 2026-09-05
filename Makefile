CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Isrc

SRC_DIR = src
BUILD_DIR = build

ALL_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
ALL_HDRS := $(wildcard $(SRC_DIR)/*.h)
ALL_CODE_FILES := $(ALL_SRCS) $(ALL_HDRS)

MAIN_SRC := $(SRC_DIR)/main.cpp
TEST_SRC := $(SRC_DIR)/Tests.cpp

COMMON_SRCS := $(filter-out $(MAIN_SRC) $(TEST_SRC), $(ALL_SRCS))
COMMON_OBJS := $(COMMON_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

MAIN_OBJ := $(MAIN_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ := $(TEST_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean tests format lint

all: $(BUILD_DIR)/at_server

$(BUILD_DIR)/at_server: $(MAIN_OBJ) $(COMMON_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

tests: $(BUILD_DIR)/tests
	@./$(BUILD_DIR)/tests

$(BUILD_DIR)/tests: $(TEST_OBJ) $(COMMON_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

format:
	@echo "Formatting code with clang-format..."
	clang-format -i $(ALL_CODE_FILES)

lint:
	@echo "Analyzing code with clang-tidy..."
	clang-tidy $(ALL_SRCS) -- $(CXXFLAGS)

clean:
	rm -rf $(BUILD_DIR)
