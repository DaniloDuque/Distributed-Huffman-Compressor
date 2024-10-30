# Name of the executable
TARGET = decompress

# Compiler
CC = gcc

# Compilation flags
CFLAGS = -Wall -Wextra -O2

# Source file
SRC = run.c

# Default rule
all: $(TARGET)

# Rule to compile the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Rule to clean generated files
clean:
	rm -f $(TARGET)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)
