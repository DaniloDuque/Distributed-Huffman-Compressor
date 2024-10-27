#ifndef TEST_COMPRESSOR
#define TEST_COMPRESSOR
#include "../util.h"

extern int dto[MAX_SIZE*2];
mutex cmp = PTHREAD_MUTEX_INITIALIZER;

void* sendRoutes(void* arg) {
    int* exitCode = (int*)malloc(sizeof(int));
    client_info* info = (client_info*) arg;
    if (!exitCode) {
        perror("Error in malloc");
        close(info->socket);
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }

    if(send(info->socket, dto, 2*MAX_SIZE*sizeof(int), 0) < 0) {
        perror("Error sending the code table");
        *exitCode = -1;
        close(info->socket);
        exit_t(exitCode);
        return NULL;
    }

    ll partSize;
    ssize_t size_received = 0;
    while (size_received < sizeof(partSize)) {
        ssize_t received = recv(info->socket,
                              (char*)&partSize + size_received,
                              sizeof(partSize) - size_received,
                              0);
        if (received <= 0) {
            perror("Error receiving the compressed part size");
            *exitCode = -1;
            close(info->socket);
            exit_t(exitCode);
            return NULL;
        }
        size_received += received;
    }
    
    printf("Expected bytes of compressed part: %lld\n", partSize);

    char ruta[0x19];
    sprintf(ruta, "%s%d", SAVED_FILE_ROUTE, info->index);
    fprintf(stderr, "Writing to file: %s\n", ruta);
    
    FILE* filePart = fopen(ruta, "wb");
    if (filePart == NULL) {
        perror("Error creating the compressed part file");
        *exitCode = -1;
        close(info->socket);
        exit_t(exitCode);
        return NULL;
    }

    ll total_received = 0;
    uchar buffer[BUFFER_SIZE];
    
    while (total_received < partSize) {
        size_t remaining = partSize - total_received;
        size_t to_receive = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
        
        ssize_t bytes_received = 0;
        size_t current_received = 0;

        while (current_received < to_receive) {
            bytes_received = recv(info->socket,
                                buffer + current_received,
                                to_receive - current_received,
                                0);
            
            if (bytes_received <= 0) {
                fprintf(stderr, "Connection error while receiving data\n");
                *exitCode = -1;
                fclose(filePart);
                close(info->socket);
                exit_t(exitCode);
                return NULL;
            }
            
            current_received += bytes_received;
        }
        
        size_t written = fwrite(buffer, 1, current_received, filePart);
        if (written != current_received) {
            fprintf(stderr, "Error writing to file\n");
            *exitCode = -1;
            fclose(filePart);
            close(info->socket);
            exit_t(exitCode);
            return NULL;
        }
        
        total_received += current_received;
        printf("Progress: %lld/%lld bytes received\n", total_received, partSize);
    }

    if (total_received != partSize) {
        fprintf(stderr, "Bytes do not match, received: %lld expected: %lld\n",
                total_received, partSize);
        *exitCode = -1;
        fclose(filePart);
        close(info->socket);
        exit_t(exitCode);
        return NULL;
    }

    fclose(filePart);
    close(info->socket);
    *exitCode = 0;
    exit_t(exitCode);
    return NULL;
}

bool compressFile(int n, client_info* clients) {
    if (!clients) return false;
    
    thread threads[n];
    bool flag = true;
    
    for (int i = 0; i < n; i++) {
        if (create(&threads[i], NULL, &sendRoutes, &clients[i]) != 0) {
            perror("Error creating threads for compression");
            return false;
        }
    }
    
    int* exitCode;
    for (int i = 0; i < n; i++) {
        if (join(threads[i], (void**)&exitCode) != 0 || *exitCode != 0) {
            fprintf(stderr, "Thread %d crashed while compressing\n", i);    
            flag = false;
        }
    }
    
    free(exitCode);
    return flag;
}
#endif