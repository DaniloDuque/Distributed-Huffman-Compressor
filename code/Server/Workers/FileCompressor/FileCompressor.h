#ifndef FILE_COMPRESSOR
#define FILE_COMPRESSOR

#include "../util.h"

bool compress(int* codes, int socket) {
    // if (!codes) return false;
    
    // int byte = 0;
    // int bit = 7;
    // FILE* fileR = fopen(OUTPUT_FILE, "rb");    
    // if(fileR == NULL) {
    //     perror("Error al abrir el archivo de lectura");
    //     return false;
    // }
    
    // FILE* fileW = fopen(PATH_COMPRESS, "wb");
    // if(fileW == NULL) {
    //     perror("Error al abrir el archivo de escritura");
    //     fclose(fileR);
    //     return false;
    // }
        
    // size_t bytesRead;

    // uchar buffer[BUFFER_SIZE];
    // uchar cbuffer[BUFFER_SIZE];

    // size_t compressPos;

    // unsigned char byte = 0;
    // int bit = 7;

    // size_t totalBytesWritten = 0;

    // while((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, fileR)) > 0) {
    //     compressPos = 0;
        
    //     if (ferror(fileR)) {
    //         perror("Error al leer el archivo de entrada");
    //         fclose(fileW);
    //         fclose(fileR);
    //         return false;
    //     }

    //     for(size_t i = 0; i < bytesRead; i++) {
    //         unsigned int ascii = (unsigned int)buffer[i];
            
    //         if (codes[2*ascii] <= 0 || codes[2*ascii] > MAX_SIZE) {
    //             fprintf(stderr, "Código inválido encontrado para el carácter %d\n", ascii);
    //             fclose(fileW);
    //             fclose(fileR);
    //             return false;
    //         }

    //         for(int j = 0; j < codes[2*ascii]; j++) {
    //             if (compressPos >= BUFFER_SIZE - 1) {
    //                 if (fwrite(cbuffer, 1, compressPos, fileW) != compressPos) {
    //                     perror("Error al escribir en el archivo comprimido");
    //                     fclose(fileW);
    //                     fclose(fileR);
    //                     return false;
    //                 }
    //                 totalBytesWritten += compressPos;
    //                 compressPos = 0;
    //             }

    //             if(TEST(codes[2*ascii+1], j)) SET(byte, bit);
    //             bit--;
                
    //             if(bit == -1) {
    //                 cbuffer[compressPos++] = byte;
    //                 byte = 0;
    //                 bit = 7;
    //             }
    //         }
    //     }
        
    //     if (compressPos > 0) {
    //         if (fwrite(cbuffer, 1, compressPos, fileW) != compressPos) {
    //             perror("Error al escribir en el archivo comprimido");
    //             fclose(fileW);
    //             fclose(fileR);
    //             return false;
    //         }
    //         totalBytesWritten += compressPos;
    //     }
    // }

    // if (bit != 7) {
    //     unsigned char finalBytes[2];
    //     finalBytes[0] = byte;
    //     finalBytes[1] = 7 - bit;
        
    //     if (fwrite(finalBytes, 1, 2, fileW) != 2) {
    //         perror("Error al escribir los bits finales en el archivo comprimido");
    //         fclose(fileW);
    //         fclose(fileR);
    //         return false;
    //     }
    //     totalBytesWritten += 2;
    // }

    // printf("Compresión completada. Total de bytes escritos: %zu\n", totalBytesWritten);
    
    // if(remove(OUTPUT_FILE) != 0) {
    //     perror("Error al eliminar el archivo");
    //     return false;
    // }
 
    // fileW = fopen(PATH_COMPRESS,"rb");
    // if(fileW == NULL){
    //     perror("Error al abrir el archivo antes de enviar");
    //     return false;
    // }
    
    // fseek(fileW,0,SEEK_END);
    // ll file_size = ftell(fileW);
    // fseek(fileW, 0, SEEK_SET);
    // if(send(socket, &file_size , sizeof(file_size), 0) < 0){
    //     perror("Error al enviar el size");
    //     return false;
    // }
    // printf("Bytes send of compressed part %lld\n", file_size);
    
    // ll remainingBytes = file_size;
    // ll totalSent = 0;
    // while(remainingBytes > 0){
    //     size_t bytesToRead = (remainingBytes < BUFFER_SIZE) ? remainingBytes : BUFFER_SIZE;
    //     size_t bytesRead = fread(buffer, 1, bytesToRead, fileW);
        
    //     if(bytesRead <= 0) {
    //         if(feof(fileW)) break;  
    //         perror("Error sending file");
    //         return false;
    //     }
    //     ssize_t bytesSent = send(socket, buffer, bytesRead, 0);
    //     if(bytesSent == -1) {
    //         perror("Error while sending file");
    //         return false;
    //     }
    //     totalSent += bytesSent;
    //     remainingBytes -= bytesSent;
    // }
    // if(totalSent != file_size) {
    //     fprintf(stderr, "Error: Sent %lld bytes of %lld expected\n", totalSent,file_size);
    //     return false;
    // }   
    
    // // if(remove(PATH_COMPRESS) != 0) {
    // //     perror("Error al eliminar el archivo");
    // //     return false;
    // // }
    
//    return true;
//}


    if (!codes) return false;
    
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
        for(int i = 0; i < bytesRead && compressPos < BUFFER_SIZE; i++) {
            unsigned int ascii = (unsigned int)buffer[i];
            if (ascii >= MAX_SIZE) continue;
            for(int j = 0; j < codes[2*ascii] && compressPos < BUFFER_SIZE; j++) {
                if(TEST(codes[2*ascii+1], j)) SET(byte, bit);
                bit--;
                if(bit == -1) {
                    cbuffer[compressPos] = byte;
                    byte = 0;
                    compressPos++;
                    bit = 7;
                }
            }
        }
        if(compressPos > 0) {
            fwrite(cbuffer, 1, compressPos, fileW);
        }
    }
    
    cbuffer[0] = byte;
    cbuffer[1] = (bit+1)%8;
    fwrite(cbuffer, 1, 2, fileW);
    
    fclose(fileW);
    fclose(fileR);
    
    // if(remove(OUTPUT_FILE) != 0) {
    //     perror("Error al eliminar el archivo");
    //     return false;
    // }
    
    return true;
}
#endif

