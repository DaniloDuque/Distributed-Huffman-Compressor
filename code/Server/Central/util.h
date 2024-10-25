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

#define PORT 8585
#define BUF_SIZE 1024 
#define FILE_PATH "./resources/index.html"

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
