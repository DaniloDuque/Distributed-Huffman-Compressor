// servidor_central.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define PORT 8585

void send_file(int client_socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error abriendo el archivo %s\n", filename);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    // Primero enviamos el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    send(client_socket, &file_size, sizeof(file_size), 0);

    // Luego enviamos el archivo en chunks
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Crear socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creando socket");
        exit(1);
    }

    // Configurar dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error en bind");
        exit(1);
    }

    // Listen
    if (listen(server_socket, 5) == -1) {
        perror("Error en listen");
        exit(1);
    }

    printf("Servidor central escuchando en el puerto %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Error en accept");
            continue;
        }

        printf("Cliente conectado desde %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));

        send_file(client_socket, "./resources/index.html");
        
        close(client_socket);
    }

    close(server_socket);
    return 0;
}