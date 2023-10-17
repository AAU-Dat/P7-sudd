CC = gcc
CFLAGS = -Wall -g
LIBS = -lcudd -lm -lcheck -lsubunit # Assuming 'cudd' requires linking with '-lcudd'. Adjust as needed.

# Source Files
BASE_SRCS = jajapy/base/symbolic_to_numeric.c
TEST_SRCS = jajapy/tests/test_symbolic_to_numeric.c
MAIN_SRCS = jajapy/tests/tests.c

# Object Files (inside the build folder)
BASE_OBJS = $(BASE_SRCS:.c=.o)
BASE_OBJS := $(addprefix build/, $(notdir $(BASE_OBJS)))

TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_OBJS := $(addprefix build/, $(notdir $(TEST_OBJS)))

MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_OBJS := $(addprefix build/, $(notdir $(MAIN_OBJS)))

# Targets
all: build/tests

# Compile the main test executable
build/tests: $(BASE_OBJS) $(TEST_OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile source files to object files in the build folder
build/%.o: jajapy/base/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: jajapy/tests/%.c | build
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

.PHONY: all run-tests clean
