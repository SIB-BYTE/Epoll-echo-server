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
#include "/home/layta/Desktop/debug_API/C/debug.h" // For LASSERT()

// Epoll sheet:
#define MAX_EVENTS 100
#define set_nonblocking(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

enum user_state {
	USER_OFFLINE,
	USER_ONLINE,
	USER_IDLE
};

typedef struct users {
	char *username;
	int time_active, user_state;
} user_t;

// Socket setup:
int setup_socket(int, char *, short);

// Data stream handling:
void handle_data(int);
void handle_disconnection(int);

// Epoll user creation:
void register_user(int, user_t *);

// Epoll sheet:
void setup_epoll(int);

#endif

