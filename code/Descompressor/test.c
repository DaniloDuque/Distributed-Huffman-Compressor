#include "../config.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TEST(msk, i) ((msk)&(1<<(i)))
#define SET(msk, i) ((msk)|=(1<<(i)))
#define MAX_SIZE (1<<16)
#define uchar unsigned char

int existsByte[MAX_SIZE];
int lengthTable[MAX_SIZE];
int lenTable[256];
int pathTable[256];

int main(int argc, char const *argv[]) {
    // Abrir tabla de Huffman
    FILE* huffman_table = fopen(PATH_FOR_TABLE, "rb");
    if(huffman_table == NULL) {
        perror("Error opening the huffman table file");
        return 1;
    }

    // Inicializar tablas
    memset(existsByte, -1, sizeof(existsByte));
    for(int i = 0; i < 256; i++) {
        fscanf(huffman_table, "%d %d\n", &lenTable[i], &pathTable[i]);
        if(lenTable[i]) {
            existsByte[pathTable[i]] = i;
            lengthTable[pathTable[i]] = lenTable[i];
        }
    }
    fclose(huffman_table);

    // Abrir archivo comprimido
    FILE* fileRead = fopen(PATH_FOR_COMPRESS, "rb");
    if(fileRead == NULL) {
        perror("Error opening the compressed file");
        return 1;
    }

    // Abrir archivo para descomprimir
    FILE* fileWrite = fopen(PATH_FOR_DESCOMPRESS, "wb");
    if(fileWrite == NULL) {
        perror("Error opening the decompressed file");
        fclose(fileRead);
        return 1;
    }

    // Obtener tamaño del archivo
    fseek(fileRead, 0, SEEK_END);
    long long file_size = ftell(fileRead) - 2; // -2 por los bytes de padding
    fseek(fileRead, 0, SEEK_SET);

    int byte = 0;
    int bitPos = 0;
    uchar byteRead;

    // Procesar archivo
    for(long long i = 0; i < file_size; i++) {
        if(fread(&byteRead, 1, 1, fileRead) != 1) {
            perror("Error reading compressed file");
            break;
        }

        for(int j = 7; j >= 0; j--) {
            if(byteRead & (1 << j)) {
                byte |= (1 << bitPos);
            }
            bitPos++;

            if(existsByte[byte] != -1) {
                uchar outByte = (uchar)existsByte[byte];
                fwrite(&outByte, 1, 1, fileWrite);
                byte = 0;
                bitPos = 0;
            }
        }
    }

    // Leer padding
    uchar padding, totalPadding;
    fread(&padding, 1, 1, fileRead);
    fread(&totalPadding, 1, 1, fileRead);

    // Cerrar archivos
    fclose(fileRead);
    fclose(fileWrite);

    return 0;
}