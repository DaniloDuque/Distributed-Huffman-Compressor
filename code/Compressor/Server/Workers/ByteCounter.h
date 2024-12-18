#ifndef BYTE_COUNTER
#define BYTE_COUNTER
#include "WorkerSender.h"

bool calc_frequency(int socket) {
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    ll freq[MAX_SIZE] = {};
    FILE* file = fopen(OUTPUT_FILE, "rb");
    if (file == NULL) {
        perror("Error opening the file");
        return false;
    }

    while ((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            freq[(uchar)buffer[i]]++;
        }
    }
    fclose(file);
    if (send(socket, (char*)freq, MAX_SIZE * sizeof(ll), 0) < 0) {
        perror("Error sending the frequency");
        return false;
    }
    return true;
}

#endif
