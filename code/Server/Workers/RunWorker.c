#include "ByteCounter.h"
#include "FileCompressor/FileCompressor.h"

int main() {
    int server_socket;
    struct sockaddr_in server_addr;
    socklen_t client_len = sizeof(struct sockaddr_in);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    struct hostent *server = gethostbyname(IP);
    if (server == NULL) {
        fprintf(stderr, "Error: cannot resolve hostname\n");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NGROK_PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    
    if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to central server");
        exit(1);
    }

    printf("Connected. Receiving file...\n");
    if (receive_file(server_socket) == false) {
        perror("Error receiving the file");
    }

    if (calc_frequency(server_socket) == false) {
        perror("Error calculating frequencies");
    }
    
    int codes[2 * MAX_SIZE];

    if (recv(server_socket, codes, 2 * MAX_SIZE * sizeof(int), 0) == -1) {
        perror("Error receiving the code tables");
        return 1;
    }
    
    if (compress(codes, server_socket) == false) {
        perror("Error compressing the file");
        return 1;
    }
    while(1);
    close(server_socket);
    return 0;
}
