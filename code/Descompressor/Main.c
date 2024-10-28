#include "../config.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TEST(msk, i) ((msk)&(1<<(i)))
#define SET(msk, i) ((msk)|=(1<<(i)))
#define MAX_SIZE (1<<16)
#define uchar unsigned char
#define BUFFER_SIZE (1<<13)
#define ll long long

int existsByte[MAX_SIZE]; 
int lengthTable[MAX_SIZE]; 
int lenTable[256];
int pathTable[256];

int reversed(int pos){
    int msk=0;
    int bit=1;
    for(int i=lenTable[pos]-1; i>=0; i--){
        if(pathTable[pos]&(1<<i)) msk|=bit;
        bit<<=1;
    }
    return msk;
}

int main(int argc, char const *argv[]) {
    FILE* huffman_table=fopen(PATH_FOR_TABLE, "rb");
    if(huffman_table==NULL){
        perror("Error opening the huffman table file");
        return 1;
    }
    memset(existsByte, -1, sizeof(existsByte));
    for(int i=0; i<256; i++){
        fscanf(huffman_table, "%d %d\n", &lenTable[i], &pathTable[i]);
        int mskR=reversed(i);
        if(lenTable[i]){
            existsByte[mskR]=i;
            lengthTable[mskR]=lenTable[i];
        }
    }
    fclose(huffman_table);
    FILE* fileRead=fopen(PATH_FOR_COMPRESS, "rb");
    if(fileRead==NULL){
        perror("Error opening the compressed file");
        return 1;
    }
    FILE* fileWrite=fopen(PATH_FOR_DESCOMPRESS, "wb");
    if(fileWrite==NULL){
        perror("Error opening the descompressed file");
        return 1;
    }
    fseek(fileRead, 0, SEEK_END);
    ll file_size = ftell(fileRead) - 2;
    fseek(fileRead, 0, SEEK_SET);  
    int byte = 0;
    int bitPos = 0;
    ll pos;
    uchar buffer[BUFFER_SIZE];
    uchar wbuffer[BUFFER_SIZE];
    while(file_size>0){
        pos=0;
        size_t bytes_to_read = (file_size < BUFFER_SIZE) ? file_size : BUFFER_SIZE;
        size_t bytes_read = fread(buffer, 1, bytes_to_read, fileRead);
        for(ll i=0; i<bytes_read; i++){
            uchar byteRead=buffer[i];
            for(int j = 7; j>=0; j--){
                byte|=(byteRead&(1<<j)) & (1<<bitPos);
                bitPos++;
                if(existsByte[byte]!=-1){
                    wbuffer[pos]=existsByte[byte];
                    byte=0;
                    bitPos=0;
                    pos++;
                }
            }
        }
        fwrite(wbuffer, 1, pos, fileWrite);
        file_size -= bytes_read;
    }

    uchar padding=0, totalPadding=0;
    int hasPadding=0;
    fseek(fileRead,-2,SEEK_END);
    fread(&padding,sizeof(uchar),1,fileRead); 
    fread(&totalPadding,sizeof(uchar),1,fileRead);
    fclose(fileRead);
    if(totalPadding) hasPadding=1;
    if(!hasPadding && bitPos){
        perror("Inconsistent file");
        return 1;
    }
    for(int j=7; j>=0 && totalPadding; j--, totalPadding--){
        byte|=(padding&(1<<j)) & (1<<bitPos);
        bitPos++;
    }
    if(existsByte[byte]!=-1){
        padding=existsByte[byte];
        fwrite(&padding, 1, 1, fileWrite);
    }
    else{
        perror("Inconsistent padding");
        return 1;
    }
    fclose(fileWrite);
    printf("File descompressed successfully\n");
    return 0;
}
