#ifndef FILE_CHUNKER
#define FILE_CHUNKER

#include "../util.h"
#define RESOURCE_PATH "../resources/test.txt"

mutex m = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    ll basePosition;
    ll chunkSize;
    char* path;
    client_info* client;
} params;

params* createParams(ll i, ll j, char* path, client_info* client_info){
    params* p = (params*)malloc(sizeof(params)); 
    p->basePosition = i;
    p->chunkSize = j;
    p->path = path;
    p->client = client_info;
    return p;
}

void* filePart(void* arg) {
    params* p = (params*)arg;
    int* exitCode = (int*)malloc(sizeof(int));
    FILE* file;
    file = fopen(p->path,"rb");
    if(file == NULL){
        perror("Error while opening file");
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }
    fseek(file, p->basePosition, SEEK_SET);
    uchar* filePart = (uchar*)malloc(p->chunkSize*(sizeof(uchar)));
    size_t bytesRead = fread(filePart, sizeof(uchar), p->chunkSize, file);
    if(bytesRead<(size_t)p->chunkSize){
        perror("Error while reading my part");
        *exitCode = -1;
        exit_t(exitCode);
        return NULL;
    }
    fclose(file);

    if (send(p->client->socket, &p->chunkSize, sizeof(p->chunkSize), 0) < 0) {
        perror("Error sending size");
        *exitCode = -1;
        exit_t(exitCode);
    }

    if (send(p->client->socket, filePart, p->chunkSize, 0) < 0) {
        perror("Error sending data");
        *exitCode = -1;
        exit_t(exitCode);
    }

    close(p->client->socket);
    *exitCode = 0;
    exit_t(exitCode);
    return NULL;
}

void* splitFile(void* spd){
    splitFileDTO *sp = (splitFileDTO*)spd;
    FILE* file;
    file = fopen(sp->path,"rb");
    if(file == NULL){
        perror("Error while opening file");
        return NULL;
    }
    fseek(file,0,SEEK_END);
    ll size = ftell(file), chunkSize = (size / sp->numServers);
    fclose(file);
    thread threads[sp->numServers];
    ll residual = size % sp->numServers, basePos = 0;
    for(ll i = 0; i < sp->numServers; i++) {
        params* param = createParams(basePos, 
            chunkSize+(residual>0), sp->path, &sp->client[i]);
        fprintf(stderr, "Base: %lld Total: %lld\n", param->basePosition, param->chunkSize);        
        basePos+=chunkSize+(residual>0);
        if(create(&threads[i], NULL, &filePart, param)!=0){
            perror("Error creating the thread in splitFile");
            return NULL;
        }
        detach(threads[i]);
        residual--;
    }
}

#endif