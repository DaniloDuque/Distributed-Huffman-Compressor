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
    fprintf(stderr,"Size of expected compressed part %lld\n", sizeFile);

    char ruta[25];
    sprintf(ruta, "%s%d", SAVED_FILE_ROUTE, info->index);
    fprintf(stderr, "Writing to file: %s\n", ruta);

    FILE* file=fopen(ruta, "wb");
    
    if(file==NULL){
        perror("Error creating the compressed file");
        *exitCode = -1;
        close(socket);
        exit_t(exitCode);
    }

    ll total_received = 0;
    ssize_t bytes_received;
    
    uchar buffer[BUFFER_SIZE];

    while (total_received < sizeFile && 
           (bytes_received = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
        total_received += bytes_received;
        // printf("Progress: %.2f%%\n", (float)total_received / sizeFile * 100);
    }
    if(sizeFile!=total_received){
        perror("Error: amount of bytes not equal");
        *exitCode = -1;
        fclose(file);
        close(socket);
        exit_t(exitCode);
    }
    printf("File received and saved as %s\n", ruta);

    fclose(file);
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