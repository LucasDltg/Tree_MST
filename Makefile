CC ?= gcc
CFLAGS ?= -Wall -Wextra -g
LDFLAGS?= -ltps -lSDL2 -lSDL2_ttf

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Target executable
TARGET = trees

# Main rule to build the target executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile each source file into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

doc:
	doxygen Doxyfile

# Rule to clean up generated files
clean:
	rm -rf $(BUILD_DIR)

clean-doc:
	rm -rf docs

.PHONY: clean