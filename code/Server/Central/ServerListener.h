#ifndef SERVER_LISTENER
#define SERVER_LISTENER

#include "MakeTable.h"

extern mutex matex;

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

bool receiveAndUpdateTable(int socket){
    ll table[MAX_SIZE] = {};
    if(recv(socket, table, MAX_SIZE * sizeof(ll), 0) < 0){
        perror("Error al recibir la tabla");
        return false;
    }
    perror("Table received");
    updateTable(table);
    perror("Table updated");
    return true;
}

// void sendTable(int client_socket, ll* table){
//     send(client_socket, table, sizeof(table), 0);
//     fprintf(stdout, "Send table with code 0;");
// }


#endif