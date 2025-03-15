# Cardboard-Pirates -------- https://github.com/quonverbat/Cardboard-Pirates
SHELL := /bin/sh
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src
SRC := $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o)
DEP := $(OBJ:.o=.d)
INCS := -Iinclude
SDL_LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LDFLAGS := $(SDL_LIBS) -ldl
CPPFLAGS := -MMD -MP
CXXFLAGS := -std=c++23 -O2 $(INCS) $(CPPFLAGS)
CXX := g++

all: Cardboard-Pirates

Cardboard-Pirates: $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $(BIN_DIR)/$@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all Cardboard-Pirates clean
-include $(DEP)
