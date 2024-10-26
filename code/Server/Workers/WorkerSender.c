#include "util.h"

#define IP "0.tcp.ngrok.io"
#define NGROK_PORT 15113 // Puerto proporcionado por ngrok

void receive_file(int socket) {
    char buffer[BUFFER_SIZE];
    FILE* file = fopen(OUTPUT_FILE, "wb");
    if (file == NULL) {
        printf("Error creating output file\n");
        return;
    }

    ll file_size;
    recv(socket, &file_size, sizeof(file_size), 0);
    printf("Size to file: %lld bytes\n", file_size);

    long total_received = 0;
    ssize_t bytes_received;

    while (total_received < file_size && 
           (bytes_received = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
        total_received += bytes_received;
        printf("Progress: %.2f%%\n", (float)total_received / file_size * 100);
    }

    fclose(file);
    printf("File received and saved as %s\n", OUTPUT_FILE);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    struct hostent * servidor = gethostbyname(IP);
    if (servidor == NULL) {
        fprintf(stderr, "Error: cannot resolve hostname\n");
        return 1;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NGROK_PORT);
    memcpy(&server_addr.sin_addr.s_addr, servidor->h_addr_list[0], servidor->h_length);
    
    if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to central server");
        exit(1);
    }

    printf("Connected. Receiving file...\n");

    receive_file(server_socket);

    close(server_socket);
    return 0;
}