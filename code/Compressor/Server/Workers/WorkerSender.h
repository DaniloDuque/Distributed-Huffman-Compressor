#include "util.h"

bool receive_file(int socket) {
    char buffer[BUFFER_SIZE];
    FILE* file = fopen(OUTPUT_FILE, "wb");
    if (file == NULL) {
        printf("Error creating output file\n");
        return false;
    }

    ll file_size;
    if(recv(socket, &file_size, sizeof(file_size), 0)==-1){
        printf("Error receiving the part size\n");
        return false;
    }
    printf("Size to file: %lld bytes\n", file_size);

    long total_received = 0;
    ssize_t bytes_received;

    while (total_received < file_size && 
           (bytes_received = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
        total_received += bytes_received;
        printf("Progress: %.2f%%\n", (float)total_received / file_size * 100);
    }
    if(file_size!=total_received){
        perror("Error: los bytes no coinciden");
        return false;
    }
    printf("File received and saved as %s\n", OUTPUT_FILE);

    fclose(file);
    return true;
}