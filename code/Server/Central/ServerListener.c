#include "util.h"

#define PORT_T 8585


void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;
    
    read_size = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (read_size > 0) {
        buffer[read_size] = '\0';  
        printf("Request:\n%s\n", buffer);
        int file_fd = open(FILE_PATH, O_RDONLY);
        if (file_fd < 0) {
            perror("Error opening file");
            write(client_socket, error_response, strlen(error_response));
        } else {
            struct stat file_stat;
            if (fstat(file_fd, &file_stat) < 0) {
                perror("Error getting file stats");
                close(file_fd);
                write(client_socket, error_response, strlen(error_response));
            } else {
                char header[BUFFER_SIZE];
                snprintf(header, sizeof(header), header_template, file_stat.st_size);               
                write(client_socket, header, strlen(header));
                char file_buffer[BUFFER_SIZE];
                int bytes_read;
                while ((bytes_read = read(file_fd, file_buffer, BUFFER_SIZE)) > 0) {
                    write(client_socket, file_buffer, bytes_read);
                }
                close(file_fd);
            }
        }
    }
    
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_socket, 5) < 0) {
        perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("HTTP server listening on port %d...\n", PORT);
    
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }
        handle_client(client_socket);
    }
    
    close(server_socket);
    return 0;
}
