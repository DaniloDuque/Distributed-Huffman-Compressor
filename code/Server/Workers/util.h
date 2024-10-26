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

#define BUFFER_SIZE 1024
#define NGROK_PORT 18122 // Puerto proporcionado por ngrok
#define OUTPUT_FILE "received_index.html"

#endif