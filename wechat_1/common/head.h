#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct Node {
    struct sockaddr_in addr;
    char name[20];
    struct Node *next;
} Node, *LinkedList;

#ifdef _DEBUG
#define D(fmt, args...) printf (fmt, ##args)
#else
#define D(fmt, args...)
#endif

#endif
