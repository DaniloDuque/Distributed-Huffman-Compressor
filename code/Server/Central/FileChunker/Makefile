# Compiler name
CC = gcc

# Compilation flags
CFLAGS = -Wall -Wextra -std=c99

# Source file
SRC = FileChunker.c

# Executable name
TARGET = file_chunker

# Default rule
all: $(TARGET)

# Rule to compile the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Rule to clean compiled files
clean:
	rm -f $(TARGET)

# Rule to rebuild the project from scratch
rebuild: clean all
