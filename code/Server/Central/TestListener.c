#include "util.h"

#define PORT 12345
#define BUFFER_SIZE 1024

int worker_socket = -1;  

void *handle_client_worker(void *client_sock) {
    int client_socket = *(int *)client_sock;
    free(client_sock);
    
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen("received_file_from_client.txt", "wb");

    if (fp != NULL) {
        while (1) {
            int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) break;
            fwrite(buffer, 1, bytes_received, fp);
        }
        fclose(fp);
        printf("Archivo recibido del cliente/trabajador\n");
    }

    FILE *file_to_send = fopen(FILE_PATH, "rb");
    if (file_to_send != NULL) {
        while (1) {
            size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file_to_send);
            if (bytes_read <= 0) break;
            send(client_socket, buffer, bytes_read, 0);
        }
        fclose(file_to_send);
        printf("Archivo reenviado al cliente/trabajador\n");
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error al crear socket del servidor");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al enlazar socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error al escuchar conexiones");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor central escuchando en el puerto %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
        if (client_socket == -1) {
            perror("Error al aceptar conexión");
            continue;
        }

        pthread_t thread;
        int *client_sock = malloc(sizeof(int));
        *client_sock = client_socket;
        pthread_create(&thread, NULL, handle_client_worker, client_sock);
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}