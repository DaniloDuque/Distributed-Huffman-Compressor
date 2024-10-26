SRC = ServerListener.c

EXEC = server

CC = gcc
CFLAGS = -Wall -Wextra

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC)

help:
	@echo "Available commands:"
	@echo "  make        - Compiles the server"
	@echo "  make run    - Compiles and runs the server"
	@echo "  make clean  - Removes the executable"
	@echo "  make help   - Displays this help message"
