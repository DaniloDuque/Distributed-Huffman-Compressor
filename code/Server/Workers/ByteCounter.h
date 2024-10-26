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

    if(send(socket, MAX_SIZE, sizeof(MAX_SIZE), 0) == -1) {
        perror("Error while sending chunk size");
        return false;
    }

    if(send(socket, freq, MAX_SIZE, 0) < 0){
        perror("Error enviando la frecuencia");
        return false;
    }
    return true;
}

#endif