#include "../util.h"

#define RESOURCE_PATH "../resources/hola.txt"

typedef struct{
    ll basePosition;
    ll chunkSize;
} params;

params* createParams(ll i, ll j){
    params* p = (params*)malloc(sizeof(params)); 
    p->basePosition = i;
    p->chunkSize = j;
    return p;
}

void* filePart(FILE* file, void* arg){
    params *p = (params*)arg;
    fseek(file, start, SEEK_SET);
    uchar* filePart = (uchar*)malloc(p->chunkSize*(sizeof(uchar)));
    for(ll i = 0;i<p->chunkSize; ++i)
    {
        uchar byte = fgetc(file);
        if(byte == EOF) break;
        filePart[i] = byte;
    }
}

void splitFile(char* path, int numServers){
    FILE* file;
    file = fopen(path,"r");
    if(file == NULL){
        perror("Error al abrir el archivo");
    }
    fseek(file,0,SEEK_END);
    ll size = ftell(file), chunkSize = (size / numServers);
    fclose(file);
    thread threads[numServers]; 
    
    for(ll i = 0; i < numServers ; i++) {
        params* param = createParams(i * chunkSize, min(size, (i + 1) * chunkSize) - (i * chunkSize));
        create(&threads[i], NULL, &filePart, param);
    }
    
    for(int i = 0; i < numServers; i++){
        join(&threads[i],NULL);
    }
}

int main(){

    return 0;
}
