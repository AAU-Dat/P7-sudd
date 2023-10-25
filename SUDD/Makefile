CC = gcc
CFLAGS = -Wall -g
LIBS = -lcudd -lm -lcheck -lsubunit

TEST_DIR = tests
SRC_DIR = src
OBJ_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Targets
all: build/tests

# Compile the main test executable
build/tests: $(TEST_OBJS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile source files to object files in the build folder
build/%.o: $(SRC_DIR)/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: $(TEST_DIR)/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Run the tests
check: build/tests
	./build/tests

# Clean up
clean:
	rm -rf build/*

# Create the build directory
build:
	mkdir -p build

.PHONY: all check clean
