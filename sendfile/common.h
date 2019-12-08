#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int socket_create(int);

typedef struct Message {
    char name[20];
    int size;
    char buff[512];
} Message;

#endif
