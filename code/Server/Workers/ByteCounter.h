#ifndef BYTE_COUNTER
#define BYTE_COUNTER
#include "WorkerSender.h"

bool calc_frecuency(int socket){
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    ll freq[MAX_SIZE] = {};
    FILE * file = fopen(OUTPUT_FILE, "rb");
    if(file == NULL){
        perror("Error al abrir el archivo");
        return false;
    }

    while((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0){
        for(int i=0; i<bytesRead; i++) {
            freq[(uchar)buffer[i]]++;
        }
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