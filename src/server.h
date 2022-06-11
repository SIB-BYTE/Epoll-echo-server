#ifndef __SERVER_H
#define __SERVER_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define LASSERT(condition, message, ...) \
    do \
    { \
        if(!(condition)) \
        { \
            fprintf(stderr, "[*] Process terminated! LASSERT(); failed!\nCondition\t: {%s}\nFunction\t: {%s}\nFailed in file\t: {%s}\nAt line \t: {%d}\n", #condition, __func__, __FILE__, __LINE__);\
            fprintf(stderr, message, ##__VA_ARGS__); \
            exit(1); \
        } \
    } while(0) \

// Epoll sheet:
#define MAX_EVENTS 100
#define set_nonblocking(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

// Socket setup:
int setup_socket(int, char *, short);

// Data stream handling:
void handle_data(int);

// Epoll sheet:
void setup_epoll(int);

#endif

