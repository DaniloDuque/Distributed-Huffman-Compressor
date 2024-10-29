#ifndef FILE_CHUNKER
#define FILE_CHUNKER
#include "../util.h"
#include "../ServerListener.h"

#define RESOURCE_PATH "../resources/test.txt"

mutex m = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    ll basePosition;
    ll chunkSize;
    char* path;
    client_info* client;
} params;

params* createParams(ll i, ll j, char* path, client_info* client_info) {
    params* p = (params*)malloc(sizeof(params)); 
    p->basePosition = i;
    p->chunkSize = j;
    p->path = path;
    p->client = client_info;
    return p;
}

void* filePart(void* arg) {
    params* p = (params*)arg;
    FILE* file;
    int * exitCode = (int *) malloc(sizeof(int));
    file = fopen(p->path, "rb");
    if(file == NULL) {
        perror("Error while opening file");
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }

    fseek(file, p->basePosition, SEEK_SET);

    if(send(p->client->socket, &p->chunkSize, sizeof(p->chunkSize), 0) == -1) {
        perror("Error while sending chunk size");
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }

    unsigned char buffer[BUFFER_SIZE];
    ll remainingBytes = p->chunkSize;
    ll totalSent = 0;

    while(remainingBytes > 0) {
        size_t bytesToRead = (remainingBytes < BUFFER_SIZE) ? remainingBytes : BUFFER_SIZE;
        size_t bytesRead = fread(buffer, 1, bytesToRead, file);
        
        if(bytesRead <= 0) {
            if(feof(file)) break;  
            perror("Error sending file");
            *exitCode = -1;
            exit_t(exitCode);
            return NULL;
        }

        ssize_t bytesSent = send(p->client->socket, buffer, bytesRead, 0);
        if(bytesSent == -1) {
            perror("Error while sending file");
            *exitCode = -1;
            exit_t(exitCode);
            return NULL;
        }

        totalSent += bytesSent;
        remainingBytes -= bytesSent;
    }
    if(totalSent != p->chunkSize) {
        fprintf(stderr, "Error: Sent %lld bytes of %lld expected\n", totalSent, p->chunkSize);
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }
    fclose(file);
    if(receiveAndUpdateTable(p->client->socket) == false) {
        perror("Error receiving the table of frequencies");
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }
    *exitCode = 0;
    exit_t(exitCode);
    return NULL;
}

bool splitFile(splitFileDTO * sp) {
    FILE* file;
    file = fopen(sp->path, "rb");
    if(file == NULL) {
        perror("Error while opening file");
        return false;
    }
    fseek(file, 0, SEEK_END);
    ll size = ftell(file), chunkSize = (size / sp->numServers);
    fclose(file);
    thread threads[sp->numServers];
    ll residual = size % sp->numServers, basePos = 0;
    for(ll i = 0; i < sp->numServers; i++) {
        params* param = createParams(basePos, 
            chunkSize + (residual > 0), sp->path, &sp->client[i]);
        basePos += chunkSize + (residual > 0);
        if(create(&threads[i], NULL, &filePart, param) != 0) {
            perror("Error creating the thread in splitFile");
            return false;
        }
        residual--;
    }
    int * exitCode;
    bool flag = true;
    for(int i = 0; i < sp->numServers; i++) {
        if(join(threads[i], (void **)&exitCode) < 0 || *exitCode != 0) { 
            flag = false;
            fprintf(stderr, "Thread %d crashed\n", i);
        }
    }
    return flag;
}

#endif
