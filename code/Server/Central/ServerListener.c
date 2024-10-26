#include "FileChunker/FileChunker.h"

#define max_client 100

client_info info[max_client];

splitFileDTO* createSplitDTO(char * path, client_info* clients, int n){
    splitFileDTO* s = (splitFileDTO*) malloc(sizeof(splitFileDTO));
    s->path = path;
    s->client = clients;
    s->numServers = n;
    return s;
}

void send_file(int client_socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file at: %s\n", filename);
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
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error while binding");
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error while listening");
        exit(1);
    }

    printf("Central server listening port: %d...\n", PORT);
    int cont=0, n=3;
    while (cont<n) {
        info[cont].socket = accept(server_socket, (struct sockaddr*)&info[cont].client_addr, &info[cont].client_len);
        if (info[cont].socket == -1) {
            perror("Error in accept");
            continue;
        }
        info[cont].index = cont;
        printf("Client connected from %s:%d!\n", 
        inet_ntoa(info[cont].client_addr.sin_addr), 
        ntohs(info[cont].client_addr.sin_port));        cont++;
    }
    splitFileDTO * params = createSplitDTO(FILE_PATH, info, n);
    thread thr;
    create(&thr, NULL, &splitFile, params);
    pthread_detach(thr);
    while(1);
    close(server_socket);
    return 0;
}