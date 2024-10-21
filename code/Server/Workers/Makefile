# Compiler to use
CC = gcc

# Compilation flags
CFLAGS = -Wall -pthread

# Name of the output executable
OUTPUT = a.out

# Source file
SRC = run.c

# Default rule (build everything)
all: $(OUTPUT)

# Rule to compile the executable
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

# Clean up generated files
clean:
	rm -f $(OUTPUT)
