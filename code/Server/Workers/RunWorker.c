#include "ByteCounter.h"

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

    calc_frecuency(receive_file(server_socket),server_socket);

    close(server_socket);
    return 0;
}