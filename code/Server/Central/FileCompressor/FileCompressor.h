#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"
extern int dto[MAX_SIZE*2];
mutex dto_mutex = PTHREAD_MUTEX_INITIALIZER;
mutex cmp = PTHREAD_MUTEX_INITIALIZER;

void* sendRoutes(void* arg) {
    client_info* info = (client_info*) arg;
    int socket = info->socket;
    int* exitCode = (int*)malloc(sizeof(int));
    if (!exitCode) {
        perror("Error in malloc");
        close(socket);
        return NULL;
    }
    *exitCode = -1;  // Inicializar con error por defecto

    // Enviar tabla de códigos
    ssize_t sent_bytes = send(socket, (int*)dto, 2*MAX_SIZE*sizeof(int), 0);
    if(sent_bytes < 0 || sent_bytes != 2*MAX_SIZE*sizeof(int)) {
        perror("Error sending the code table");
        close(socket);
        return (void*)exitCode;
    }

    // Recibir tamaño del archivo
    ll sizeFile;
    ssize_t recv_size = recv(socket, &sizeFile, sizeof(sizeFile), MSG_WAITALL);
    if(recv_size != sizeof(sizeFile)) {
        perror("Error al recibir el tamaño");
        close(socket);
        return (void*)exitCode;
    }
    fprintf(stderr, "Size of expected compressed part %lld\n", sizeFile);

    // Preparar nombre del archivo
    char ruta[256];  // Buffer más grande para evitar desbordamientos
    if (snprintf(ruta, sizeof(ruta), "%s%d", SAVED_FILE_ROUTE, info->index) >= sizeof(ruta)) {
        fprintf(stderr, "Error: Nombre de archivo demasiado largo\n");
        close(socket);
        return (void*)exitCode;
    }
    fprintf(stderr, "Writing to file: %s\n", ruta);

    // Abrir archivo
    FILE* file = fopen(ruta, "wb");
    if(file == NULL) {
        perror("Error creating the compressed file");
        close(socket);
        return (void*)exitCode;
    }

    // Recibir datos
    ll total_received = 0;
    ssize_t bytes_received;
    uchar buffer[BUFFER_SIZE];
    
    //lock(&cmp);
    while (total_received < sizeFile) {
        size_t remaining = sizeFile - total_received;
        size_t to_receive = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
        
        bytes_received = recv(socket, buffer, to_receive, MSG_WAITALL);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                fprintf(stderr, "Connection closed by client\n");
            } else {
                perror("Error receiving data");
            }
            fclose(file);
            close(socket);
            return (void*)exitCode;
        }

        size_t written = fwrite(buffer, 1, bytes_received, file);
        if (written != bytes_received) {
            perror("Error writing to file");
            fclose(file);
            close(socket);
            return (void*)exitCode;
        }

        total_received += bytes_received;
        //printf("Progress: %.2f%% (%lld/%lld bytes)\n", (float)total_received / sizeFile * 100, total_received, sizeFile);
    }
    //unlock(&cmp);

    if(sizeFile != total_received) {
        fprintf(stderr, "Error: amount of bytes not equal, received: %lld, expected: %lld\n", 
                total_received, sizeFile);
        fclose(file);
        close(socket);
        return (void*)exitCode;
    }

    printf("File received and saved as %s\n", ruta);
    fclose(file);
    close(socket);
    *exitCode = 0;  // Éxito
    return (void*)exitCode;
}

bool compressFile(int n, client_info* clients) {
    if (!clients) return false;
    
    thread* threads = malloc(n * sizeof(thread));
    if (!threads) {
        perror("Error allocating thread array");
        return false;
    }
    
    bool flag = true;
    
    // Crear hilos
    for(int i = 0; i < n; i++) {
        if(create(&threads[i], NULL, &sendRoutes, &clients[i]) != 0) {
            perror("Error creating threads for compression");
            // Esperar a los hilos ya creados
            for(int j = 0; j < i; j++) {
                int* threadExitCode;
                join(threads[j], (void**)&threadExitCode);
                if (threadExitCode) free(threadExitCode);
            }
            free(threads);
            return false;
        }
    }

    // Esperar a todos los hilos
    for(int i = 0; i < n; i++) {
        int* exitCode;
        if(join(threads[i], (void**)&exitCode) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            flag = false;
        } else if(exitCode != NULL) {
            if(*exitCode != 0) {
                fprintf(stderr, "Thread %d crashed while compressing\n", i);
                flag = false;
            }
            free(exitCode);
        }
    }

    free(threads);
    return flag;
}

#endif
