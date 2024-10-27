#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"
extern int dto[MAX_SIZE*2];

mutex cmp = PTHREAD_MUTEX_INITIALIZER;

void* sendRoutes(void* arg) {
    int* exitCode = (int*)malloc(sizeof(int));
    client_info* info = (client_info*) arg;
    if (!exitCode) {
        perror("Error in malloc");
        close(info->socket);
        *exitCode=-1;
        exit_t(exitCode);
        return NULL;
    }
    if(send(info->socket, (int*)dto, 2*MAX_SIZE*sizeof(int), 0) < 0) {
        perror("Error sending the code table");
        *exitCode = -1;
        close(info->socket);
        exit_t(exitCode);
        return NULL;
    }
    ll partSize;
    if(recv(info->socket, &partSize, sizeof(partSize), 0)==-1){
        perror("Error receiving the compressed part");
        *exitCode=-1;
        close(info->socket);
        exit_t(exitCode);
        return NULL;
    }
    printf("Bytes received of compressed part %lld\n", partSize);
    char ruta[0x19];
    sprintf(ruta, "%s%d", SAVED_FILE_ROUTE, info->index); 
    fprintf(stderr,"%s\n", ruta);
    FILE* filePart=fopen(ruta, "wb");
    if(filePart==NULL){
        perror("Error creating the compressed part file\n");
        *exitCode=-1;
        close(info->socket);
        exit_t(exitCode);
        return NULL;
    }
    ll total_received=0;
    ssize_t bytes_received;
    uchar * buffer[BUFFER_SIZE];
    // lock(&cmp);
    while (total_received < partSize && 
           (bytes_received = recv(info->socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, filePart);
        total_received += bytes_received;
    }
    if(total_received!=partSize){
        fprintf(stderr, "bytes do not match, received: %lld expected: %lld\n", total_received, partSize);
        *exitCode=-1;
        close(info->socket);
        // unlock(&cmp);
        exit_t(exitCode);
        return NULL;
    }
    //unlock(&cmp);
    fclose(filePart);
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
