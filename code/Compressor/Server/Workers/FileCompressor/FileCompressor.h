#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"

bool sendCompress(int socket) {
    uchar buffer[BUFFER_SIZE];
    FILE * fileL = fopen(PATH_COMPRESS,"rb");
    if(fileL == NULL){
        perror("Error al abrir el archivo compreso");
        return false;
    }
    
    fseek(fileL, 0, SEEK_END);
    ll sizeFile = ftell(fileL);
    if (sizeFile < 0) {
        perror("Error al calcular el tamaño del archivo: archivo vacio");
        fclose(fileL);
        return false;
    }

    printf("Se van a enviar %lld bytes\n", sizeFile);
    
    if(send(socket, &sizeFile, sizeof(sizeFile), 0) < 0){
        perror("Error al enviar el tamano del compreso");
        fclose(fileL);
        return false;
    }
    printf("Send bytes %lld\n",sizeFile);

    fseek(fileL,0,SEEK_SET);
    ll remainingBytes = sizeFile;
    ll totalSent = 0;
    while(remainingBytes > 0) {
        size_t bytesToRead = (remainingBytes < BUFFER_SIZE) ? remainingBytes : BUFFER_SIZE;
        size_t bytesRead = fread(buffer, 1, bytesToRead, fileL);
        
        if(bytesRead <= 0) {
            if(feof(fileL)) break;  
            perror("Error sending file");
            return false;
        }

        ssize_t bytesSent = send(socket, buffer, bytesRead, 0);
        if(bytesSent == -1) {
            perror("Error while sending file");
            return false;
        }

        totalSent += bytesSent;
        remainingBytes -= bytesSent;
    }
    
    if(totalSent != sizeFile) {
        fprintf(stderr, "Error: Sent %lld bytes of %lld expected\n", totalSent, sizeFile);
        return false;
    }    
    fclose(fileL);
    
    if(remove(PATH_COMPRESS) != 0) {
        perror("Error al eliminar el archivo");
        return false;
    }

    return true;
}


bool compress(int* codes, int socket) {
    if (!codes) return false;
    //printf("Size of table of codes %lld\n")
    int byte = 0;
    int bit = 7;
    FILE* fileR = fopen(OUTPUT_FILE, "rb");    
    if(fileR == NULL) {
        perror("Error al abrir el archivo de lectura");
        return false;
    }
    
    FILE* fileW = fopen(PATH_COMPRESS, "wb");
    if(fileW == NULL) {
        perror("Error al abrir el archivo de escritura");
        fclose(fileR);
        return false;
    }
    
    size_t bytesRead;
    uchar buffer[BUFFER_SIZE];
    uchar cbuffer[BUFFER_SIZE];
    size_t compressPos;
    
    while((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, fileR)) > 0) {
        compressPos = 0;
        for(int i = 0; i < bytesRead; i++) {
            unsigned int ascii = (unsigned int)buffer[i];
            //printf("Asc %c Tam: %d Msk: %d\n", buffer[i], codes[2*ascii], codes[2*ascii+1]);
            if (ascii >= MAX_SIZE) printf("ups\n");
            for(int j =0; j<codes[2*ascii]; j++) {
            //for(int j=codes[2*ascii]-1; j>=0; j--){
                if(TEST(codes[2*ascii+1], j)) SET(byte, bit);
                bit--;
                if(bit == -1) {
                    cbuffer[compressPos] = byte;
                    //printf("Byte %d\n", byte);
                    byte = 0;
                    compressPos++;
                    bit = 7;
                }
            }
            //printf("\n");
        }
        if(compressPos > 0) {
            fwrite(cbuffer, 1, compressPos, fileW);
        }
    }
    cbuffer[0] = byte;
    cbuffer[1] = 7-bit;
    printf("Bits: %d total: %d bit:%d\n", cbuffer[0], cbuffer[1], bit);
    fwrite(cbuffer, 1, 2, fileW);
    
    fclose(fileW);
    fclose(fileR);

    if(remove(OUTPUT_FILE) != 0) {
        perror("Error al eliminar el archivo");
        return false;
    }

    return sendCompress(socket);
}


#endif


