#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdbool.h>

#define IP "0.tcp.ngrok.io"                            
#define NGROK_PORT 11198
#define BUFFER_SIZE (1<<14)
#define OUTPUT_FILE "received_file_part"
#define ll long long
#define uchar unsigned char
#define MAX_SIZE (1<<8)
#define PATH_COMPRESS "compressed_file"
#define Byte uchar

#define TEST(msk, i) ((msk)&(1<<(i)))
#define SET(msk, i) ((msk)|=(1<<(i)))

void displayRoute(int asc, int len, int msk){
    printf("Character: %c  Length: %d  Route: ", asc, len);
    for(int i = 0; i<len; ++i) printf("%u", (bool)TEST(msk, i));
    puts("");
}

#endif