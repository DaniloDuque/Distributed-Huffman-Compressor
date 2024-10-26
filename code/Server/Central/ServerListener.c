#include "util.h"

#define max_client 100

client_info info[max_client];

void send_file(int client_socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error abriendo el archivo %s\n", filename);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    send(client_socket, &file_size, sizeof(file_size), 0);

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creando socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error en bind");
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error en listen");
        exit(1);
    }

    printf("Servidor central escuchando en el puerto %d...\n", PORT);
    int cont=0, n=3;
    while (cont<n) {
        info[cont].socket = accept(server_socket, (struct sockaddr*)&info[cont].client_addr, &info[cont].client_len);
        if (info[cont].socket == -1) {
            perror("Error en accept");
            continue;
        }
        printf("Cliente conectado desde %s:%d\n", 
               inet_ntoa(info[cont].client_addr.sin_addr), 
               ntohs(info[cont].client_addr.sin_port));

        //send_file(client_socket, "./resources/index.html");
        cont++;
        //close(client_socket);
    }
    for(int i=0; i<cont; i++){
        send_file(info[i].socket, "./resources/index.html");
        close(info[i].socket);
    }
    close(server_socket);
    return 0;
}