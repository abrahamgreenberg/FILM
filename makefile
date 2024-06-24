# Compiler and linker settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lncurses

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
UI_SRCS = $(wildcard $(SRC_DIR)/ui/*.c)
BACKEND_SRCS = $(wildcard $(SRC_DIR)/backend/*.c)
SETTINGS_SRCS = $(wildcard $(SRC_DIR)/settings/*.c)
MAIN_SRC = $(SRC_DIR)/main.c

# Object files
UI_OBJS = $(patsubst $(SRC_DIR)/ui/%.c,$(OBJ_DIR)/ui/%.o,$(UI_SRCS))
BACKEND_OBJS = $(patsubst $(SRC_DIR)/backend/%.c,$(OBJ_DIR)/backend/%.o,$(BACKEND_SRCS))
SETTINGS_OBJS = $(patsubst $(SRC_DIR)/settings/%.c,$(OBJ_DIR)/settings/%.o,$(SETTINGS_SRCS))
MAIN_OBJ = $(OBJ_DIR)/main.o

# Targets
TARGET = file_manager

# # Build types
DEBUG_CFLAGS = -g -DDEBUG_MODE=1
RELEASE_CFLAGS = -O2 -DDEBUG_MODE=0

# # Default target
all: debug

# Debug build
debug: clean $(BIN_DIR)/$(TARGET)_debug
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(BIN_DIR)/$(TARGET)_debug

# Release build
release: clean $(BIN_DIR)/$(TARGET)
release: CFLAGS += $(RELEASE_CFLAGS)
release: $(BIN_DIR)/$(TARGET)

# Linking the executable (debug version)
$(BIN_DIR)/$(TARGET)_debug: $(UI_OBJS) $(BACKEND_OBJS) $(SETTINGS_OBJS) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Linking the executable (release version)
$(BIN_DIR)/$(TARGET): $(UI_OBJS) $(BACKEND_OBJS) $(SETTINGS_OBJS) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compiling UI source files
$(OBJ_DIR)/ui/%.o: $(SRC_DIR)/ui/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling Backend source files
$(OBJ_DIR)/backend/%.o: $(SRC_DIR)/backend/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling Settings source files
$(OBJ_DIR)/settings/%.o: $(SRC_DIR)/settings/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling Main source file
$(OBJ_DIR)/main.o: $(MAIN_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all debug release clean
