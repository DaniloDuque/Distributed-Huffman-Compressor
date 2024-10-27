#ifndef BYTE_COUNTER
#define BYTE_COUNTER
#include "WorkerSender.h"

bool calc_frecuency(FILE* file, int socket){
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    ll freq[MAX_SIZE] = {};

    while((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0){
        for(int i=0; i<BUFFER_SIZE; i++) freq[buffer[i]]++;
    }

    ll ptr = MAX_SIZE;
    fclose(file);
    
    if(send(socket, &ptr, sizeof(ptr), 0) == -1) {
        perror("Error while sending chunk size");
        return false;
    }

    if(send(socket, freq, ptr, 0) < 0){
        perror("Error enviando la frecuencia");
        return false;
    }

    for(int i = 0; i < MAX_SIZE; i++){
        printf("%c %lld\n", i, freq[i]);   
    }

    return true;
}

#endif