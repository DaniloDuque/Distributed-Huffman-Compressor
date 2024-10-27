#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"
extern int dto[MAX_SIZE*2];
mutex dto_mutex = PTHREAD_MUTEX_INITIALIZER;

void* sendRoutes(void* arg) {
    client_info* info = (client_info*) arg;
    int socket=info->socket;
    int* exitCode = (int*)malloc(sizeof(int));
    if (!exitCode) {
        perror("Error in malloc");
        *exitCode = -1;
        close(socket);
        exit_t(exitCode);
    }
    if(send(socket, (int*)dto, 2*MAX_SIZE*sizeof(int), 0) < 0) {
        perror("Error sending the code table");
        *exitCode = -1;
        close(socket);
        exit_t(exitCode);
    }
    ll sizeFile;
    if(recv(socket, &sizeFile, sizeof(sizeFile),0) < 0){
        perror("Error al recibir el tamano");
        *exitCode = -1;
        close(socket);
        exit_t(exitCode);
    }
    fprintf(stderr,"Tamaño del archivo comprimido: %lld\n", sizeFile);
    close(info->socket);
    *exitCode = 0;
    exit_t(exitCode);
}

bool compressFile(int n, client_info* clients) {
   if (!clients) return false;
    
    thread threads[n];
    bool flag = true;
    
    for(int i = 0; i < n; i++) {
        if(create(&threads[i], NULL, &sendRoutes, &clients[i]) != 0) {
            perror("Error creating threads for compression");
            return false;
        }
    }

    int* exitCode;
    for(int i = 0; i < n; i++) {
        if(join(threads[i],(void **)&exitCode) != 0 || *exitCode != 0){
            fprintf(stderr,"Thread %d crashed while compressing\n",i);    
            flag = false;
        }
    }
    free(exitCode);
    if(flag == false) return false;
    return true;
}

#endif