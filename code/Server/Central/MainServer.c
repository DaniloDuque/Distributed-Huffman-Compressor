#include "util.h"
#include "FileChunker/FileChunker.h"

client_info info[max_client];
ll freqTable[MAX_SIZE];
mutex keys[MAX_SIZE];

int main() {
    int server_socket, client_socket, n, cont = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    printf("Insert the number of client servers: ");
    scanf("%d", &n);

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
    while (cont<n) {
        info[cont].socket = accept(server_socket, (struct sockaddr*)&info[cont].client_addr, &info[cont].client_len);
        if (info[cont].socket == -1) {
            perror("Error in accept");
            continue;
        }
        info[cont].index = cont;
        printf("Client connected from %s:%d!\n", 
        inet_ntoa(info[cont].client_addr.sin_addr), 
        ntohs(info[cont].client_addr.sin_port));
        cont++;
    }

    if(splitFile(createSplitDTO(FILE_PATH,info,n))==false){
        perror("Error al dividir el archivo desde Main");
        return 1;
    }

    for(int i = 0; i < MAX_SIZE; i++){
        printf("%c %lld\n", i, freqTable[i]);   
    }

    close(server_socket);
    return 0;
}