


SRC = run.c

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
	@echo "Comandos disponibles:"
	@echo "  make        - Compila el servidor"
	@echo "  make run    - Compila y ejecuta el servidor"
	@echo "  make clean  - Elimina el ejecutable"
	@echo "  make help   - Muestra esta ayuda"

