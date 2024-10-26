#ifndef FILE_CHUNKER
#define FILE_CHUNKER
#include "../util.h"

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
    file = fopen(p->path, "rb");
    if(file == NULL) {
        perror("Error while opening file");
        return NULL;
    }

    // Posicionamos el puntero del archivo
    fseek(file, p->basePosition, SEEK_SET);

    // Enviamos primero el tamaño total del chunk
    if(send(p->client->socket, &p->chunkSize, sizeof(p->chunkSize), 0) == -1) {
        perror("Error while sending chunk size");
        return NULL;
    }

    // Buffer para leer el archivo por partes
    unsigned char buffer[BUFFER_SIZE];
    ll remainingBytes = p->chunkSize;
    ll totalSent = 0;

    // Leemos y enviamos el archivo en chunks
    while(remainingBytes > 0) {
        size_t bytesToRead = (remainingBytes < BUFFER_SIZE) ? remainingBytes : BUFFER_SIZE;
        size_t bytesRead = fread(buffer, 1, bytesToRead, file);
        
        if(bytesRead <= 0) {
            if(feof(file)) {
                break;  // Fin del archivo
            }
            perror("Error while reading file");
            return NULL;
        }

        // Enviamos el buffer
        ssize_t bytesSent = send(p->client->socket, buffer, bytesRead, 0);
        if(bytesSent == -1) {
            perror("Error while sending buffer");
            return NULL;
        }

        totalSent += bytesSent;
        remainingBytes -= bytesSent;
    }

    if(totalSent != p->chunkSize) {
        fprintf(stderr, "Warning: Sent %lld bytes of %lld expected\n", totalSent, p->chunkSize);
    }

    fclose(file);

    close(p->client->socket);
    return NULL;
}

void* splitFile(void* spd) {
    splitFileDTO *sp = (splitFileDTO*)spd;
    FILE* file;
    file = fopen(sp->path, "rb");
    if(file == NULL) {
        perror("Error while opening file");
        return NULL;
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
            return NULL;
        }
        detach(threads[i]);
        residual--;
    }
    return NULL;
}

#endif