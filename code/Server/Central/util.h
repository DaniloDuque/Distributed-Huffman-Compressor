#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../../Huffman/Tree.h"

#define PORT 8585
#define BUFFER_SIZE (1<<14)
#define FILE_PATH "./resources/JetBrainsMono.zip"
#define MAX_SIZE (1<<8)
#define SAVED_FILE_ROUTE "./resources/saved/"
#define MAX_CLIENT 100

#define create pthread_create
#define join pthread_join
#define exit_t pthread_exit
#define detach pthread_detach

#define mutex pthread_mutex_t
#define mutex_init pthread_mutex_init
#define lock pthread_mutex_lock
#define mutex_trylock pthread_mutex_trylock
#define unlock pthread_mutex_unlock
#define mutex_destroy pthread_mutex_destroy

#define cond pthread_cond_t
#define cond_init pthread_cond_init
#define cond_wait pthread_cond_wait
#define cond_signal pthread_cond_signal
#define cond_broadcast pthread_cond_broadcast
#define cond_destroy pthread_cond_destroy

#define uchar unsigned char
#define ll long long
#define thread pthread_t

#define min(a, b) ((a) < (b) ? (a) : (b))

#define size(arr) (ll)(sizeof(arr)/sizeof(arr[0]))

const char *error_response = 
    "HTTP/1.1 500 internal server error\r\n"
    "content-type: text/plain\r\n"
    "connection: close\r\n"
    "\r\n"
    "error opening file";

const char* header_template = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: %ld\r\n"
    "Connection: close\r\n"
    "\r\n";

typedef struct {
    int socket, index;
    struct sockaddr_in client_addr;
    socklen_t client_len;
} client_info;

typedef struct {
    char* path;
    client_info *client;
    int numServers;
} splitFileDTO;

#endif 
