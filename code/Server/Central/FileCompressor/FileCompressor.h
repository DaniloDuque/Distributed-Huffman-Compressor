#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"
extern int dto[MAX_SIZE*2];
// mutex dto_mutex = PTHREAD_MUTEX_INITIALIZER;

void* sendRoutes(void* arg) {
    int* exitCode = (int*)malloc(sizeof(int));
    if (!exitCode) {
        perror("Error in malloc");
        return NULL;
    }
    int socket = *((int*) arg);
    // lock(&dto_mutex);
    if(send(socket, (int*)dto, 2*MAX_SIZE*sizeof(int), 0) < 0) {
        perror("Error sending the code table");
        *exitCode = -1;
        // unlock(&dto_mutex);
        close(socket);
        return exitCode;
    }
    // unlock(&dto_mutex);
    ll partSize;
    if(recv(socket, partSize, sizeof(partSize), 0)==-1){
       perror("Error receiving the compressed part");
        *exitCode=-1;
        exit_t(exitCode);
        close(socket);
        return exitCode;
    }
    
    close(socket);
    *exitCode = 0;
    return exitCode;
}

bool compressFile(int n, client_info* clients) {
    if (!clients) return false;
    
    thread threads[n];
    bool flag = true;
    
    for(int i = 0; i < n; i++) {
        if(create(&threads[i], NULL, &sendRoutes, &clients[i].socket) != 0) {
            perror("Error creating threads for compression");
            return false;
        }
    }

    int* exitCode;
    for(int i = 0; i < n; i++) {
        if(join(threads[i],(void **)&exitCode) != 0 || *exitCode != 0){
            fprintf(stderr,"Hilo %d peto al intentar comprimir",i);    
            flag = false;
        }
    }
    free(exitCode);
    return flag;
}

#endif
