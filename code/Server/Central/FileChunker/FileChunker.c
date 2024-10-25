#include "../util.h"

#define RESOURCE_PATH "../resources/test.txt"

mutex m = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    ll basePosition;
    ll chunkSize;
    char* path;
    char* ip_private;
    char* ip_public;
    char* port;
} params;

params* createParams(ll i, ll j, char* path, char *ip){
    params* p = (params*)malloc(sizeof(params)); 
    p->basePosition = i;
    p->chunkSize = j;
    p->path = path;
    p->ip = ip;
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
    //lock(&m);
    //for(ll i = 0; i<p->chunkSize; i++) {
        //printf("%c ", filePart[i]);
    //} puts("");
    //unlock(&m);
    fclose(file);
    *exitCode = 0;
    exit_t(exitCode);
    return NULL;
}

int splitFile(char* path, char** slaves){
    FILE* file;
    file = fopen(path,"rb");
    if(file == NULL){
        perror("Error while opening file");
        return 1;
    }
    int numServers = sizeof(*slaves);
    fseek(file,0,SEEK_END);
    ll size = ftell(file), chunkSize = (size / numServers);
    fclose(file);
    thread threads[numServers];
    ll residual = size % numServers, basePos = 0;
    for(ll i = 0; i < numServers ; i++) {
        params* param = createParams(basePos, 
            chunkSize+(residual>0), path, slaves[i]);
        basePos+=chunkSize+(residual>0);
        if(create(&threads[i], NULL, &filePart, param)!=0){
            printf("Error creating the thread in splitFile\n");
            return 1;
        }
        residual--;
    }
    int* exitCode;
    bool flag = false;
    for(int i = 0; i < numServers; i++){
        if((join(threads[i], (void **)&exitCode)) != 0) flag = true;
        if(*exitCode == -1) flag = true;
    }
    return flag;
}

int main() {
    char **slaves = (char**)malloc(3 * sizeof(char*));
    slaves[0] = "127.0.0.1:8081";
    slaves[1] = "127.0.0.1:8082";
    slaves[2] = "127.0.0.1:8083";
    splitFile(RESOURCE_PATH, slaves);
    return 0;
}
