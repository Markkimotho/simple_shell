# ==============================================================================
# Makefile for Simple Shell Project
# ==============================================================================

# Disable built-in rules to prevent conflicts
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
DEBUG_CFLAGS = $(CFLAGS) -g

TARGET = hsh
OBJ_DIR = build

# Source directories
SRC_DIRS = shell_core builtins environment io_handling memory_utils \
           linked_lists string_operations error_handling history variables

# Find all source files
SRCS = $(wildcard main.c) $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
VPATH = . $(SRC_DIRS)

# Object files (flattened into OBJ_DIR)
OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(patsubst %.c,%.o,$(SRCS))))

# Include directory
INCLUDE_DIR = includes
INCLUDES = -I$(INCLUDE_DIR)

.PHONY: all clean fclean re debug dirs

# Default target
all: dirs $(TARGET)

# Create directories
dirs:
	@mkdir -p $(OBJ_DIR)

# Link executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	@echo "Build successful: ./${TARGET}"

# Compile source files
$(OBJ_DIR)/%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -MMD

# Clean targets
clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET)

fclean: clean

re: fclean all

# Debug build
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: re
	@echo "Debug build complete. Run with: gdb ./${TARGET}"

# Include dependency files
-include $(wildcard $(OBJ_DIR)/*.d)