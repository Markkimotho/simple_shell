# ==============================================================================
# Makefile for Simple Shell Project
# ==============================================================================

# --- Compiler and Flags ---
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
DEBUG_CFLAGS = $(CFLAGS) -g # Add debug symbols for debugging builds

# --- Project Directories and Files ---
TARGET = hsh
BUILD_DIR = build # Directory for object files

# List all directories containing source (.c) files
SRC_DIRS = \
	shell_core \
	builtins \
	environment \
	io_handling \
	memory_utils \
	linked_lists \
	string_operations \
	error_handling \
	history \
	variables

# Find all .c files in the root directory (main.c) and specified source directories
SRCS = $(wildcard main.c) $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Tell make where to find source files (current directory and all SRC_DIRS)
VPATH = . $(SRC_DIRS)

# Convert source file paths to object file paths, placing them in BUILD_DIR.
# This flattens all object files into the 'build/' directory.
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %.c,%.o,$(SRCS))))

# Include directory for header files
INCLUDE_DIR = includes
INCLUDES = -I$(INCLUDE_DIR)

# --- Main Targets ---

.PHONY: all $(TARGET) clean fclean re debug $(BUILD_DIR) # Added $(BUILD_DIR) back to .PHONY

# Default target: builds the executable
all: $(TARGET)

# Rule to link object files into the final executable
# The $(BUILD_DIR) is now a prerequisite for the OBJS themselves,
# ensuring it exists before any compilation.
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	@echo "Build successful: ./${TARGET}"

# Generic rule to compile .c files into .o files in the build directory.
# This rule now has $(BUILD_DIR) as a direct prerequisite.
# This is a common and reliable pattern for ensuring the directory exists.
$(BUILD_DIR)/%.o: %.c $(BUILD_DIR) # Changed from | $(BUILD_DIR) to direct prerequisite
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -MMD

# Rule to create the build directory if it doesn't exist.
# This rule's definition must appear before it is used as a prerequisite.
# Since it's .PHONY, it will always attempt to mkdir -p.
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "Created build directory: $(BUILD_DIR)"

# --- Clean Targets ---

# Removes all compiled object files and their dependency files.
clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS) $(BUILD_DIR)/*.d
	@rmdir $(BUILD_DIR) 2>/dev/null || true # Remove build directory if empty, suppress errors if not empty

# Removes everything that `clean` removes, plus the final executable.
fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(TARGET)

# Rebuilds the project from scratch (full clean then full build).
re: fclean all

# --- Debug Build Target ---
# Overrides CFLAGS to include debug symbols, then calls the 'all' target.
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: all
	@echo "Debug build complete. Run with: gdb ./${TARGET}"

# --- Include Dependency Files ---
# This line includes all automatically generated .d files.
# These files contain compilation dependencies (e.g., .c files depend on .h files).
# If a header file changes, make will know which .c files need to be recompiled.
-include $(patsubst $(BUILD_DIR)/%.o,$(BUILD_DIR)/%.d,$(OBJS))
