#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define PORT 8585
#define BUF_SIZE 1024 
#define FILE_PATH "./resources/index.html"

typedef long long ll;
typedef pthread_t thread;
typedef pthread_create create;
typedef pthread_join join;
typedef pthread_exit exit_thread;
typedef pthread_detach detach;

typedef pthread_mutex_t mutex;
typedef pthread_mutex_init mutex_init;
typedef pthread_mutex_lock mutex_lock;
typedef pthread_mutex_trylock mutex_trylock;
typedef pthread_mutex_unlock mutex_unlock;
typedef pthread_mutex_destroy mutex_destroy;

typedef pthread_cond_t cond;
typedef pthread_cond_init cond_init;
typedef pthread_cond_wait cond_wait;
typedef pthread_cond_signal cond_signal;
typedef pthread_cond_broadcast cond_broadcast;
typedef pthread_cond_destroy cond_destroy;


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

#endif 
