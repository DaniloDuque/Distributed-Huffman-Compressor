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

#define IP "6.tcp.ngrok.io"
#define NGROK_PORT 15548 
#define BUFFER_SIZE (1<<14)
#define OUTPUT_FILE "received_file_part"
#define ll long long
#define uchar unsigned char
#define MAX_SIZE (1<<8)

#endif