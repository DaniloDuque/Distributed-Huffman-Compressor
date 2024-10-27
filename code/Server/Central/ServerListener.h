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
    ll revSize;
    if(recv(socket,&revSize,sizeof(revSize),0)==-1){
        perror("Error receiving the table size");
        return false;
    }
    if(recv(socket,table,revSize,0) == -1){
        perror("Error receiving the table");
        return false;
    }
    lock(&matex);
    for(int i = 0; i < MAX_SIZE; i++){
        printf("%c %lld - ", i, table[i]);   
    }puts("");
    unlock(&matex);

    fprintf(stdout, "Table received");
    updateTable(table);
    fprintf(stdout,"Table updated");
    return true;
}   

// void sendTable(int client_socket, ll* table){
//     send(client_socket, table, sizeof(table), 0);
//     fprintf(stdout, "Send table with code 0;");
// }


#endif