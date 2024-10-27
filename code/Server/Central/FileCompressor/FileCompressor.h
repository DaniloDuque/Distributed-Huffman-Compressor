#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"

extern uchar dto[MAX_SIZE*2];

void* sendRoutes(void* arg) {
    int socket=(int) arg;
    int * exitCode = (int *) malloc(sizeof(int));
    if(send(socket, (char*)dto, 2*MAX_SIZE*sizeof(uchar), 0) < 0){
        perror("Error sending the code table");
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }
    close(socket);
    // ll partSize;
    // if(recv(socket, &partSize, sizeof(file_size), 0)==-1){
    //     printf("Error receiving the part size\n");
    //     return false;
    // }
    *exitCode = 0;
    exit_t(exitCode);
    return NULL;
}

bool compressFile(int n, client_info * clients){
    thread threads[n];
    int * exitCode;
    bool flag = true;
    for(int i = 0; i < n; i++){
        if(create(&threads[i],NULL,&sendRoutes,&clients[i].socket) != 0){
            perror("Error al crear hilos para comprimir");
            return false;
        }
    }
    for(int i = 0; i < n; i++){
        if(join(threads[i],(void**)&exitCode) != 0 || *exitCode != 0){
            perror("Hubo problemas en la ejecucion del hilo");
            flag = false;
        }
    }
    return flag;
}

#endif


