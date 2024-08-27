# Compiler and Linker
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic 

# Include directories
INCLUDES := -Iinclude

# Libraries
LIBS := -lsqlite3

# Source directories and files
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Executable name
TARGET := WhatTheHellAmIDoing

# Installation directoies
PREFIX := /usr/local
INSTALL_BIN := $(PREFIX)/bin

# Default target
.PHONY: all
all: $(BIN_DIR)/$(TARGET)

# Build executable
$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $@ $(LIBS)
	@echo "Build successful: $(BIN_DIR)/$(TARGET)"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiled: $< -> $@"

# Create directories if they do not exit
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned build files."

# Install executable
.PHONY: install
install: all
	install -d $(INSTALL_BIN)
	install -m 755 $(BIN_DIR)/$(TARGET) $(INSTALL_BIN)
	@echo "Installed $(TARGET) from $(INSTALL_BIN)"

# Uninstall Executable
.PHONY: uninstall
uninstall: 
	rm -f $(INSTALL_BIN)/$(TARGET)
	@echo "Uninstalled $(TARGET) from $(INSTALL_BIN)"

# Rebuild application
.PHONY: rebuild
rebuild: clean all
	@echo "Rebuilt application."

# Phony target to prevent conflicts with files named 'install', 'clean', etc.
.PHONY: all clean install uninstall rebuild
