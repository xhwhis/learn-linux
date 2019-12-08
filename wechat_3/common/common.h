#include "head.h"
#ifndef _COMMON_H
#define _COMMON_H

typedef struct msg {
    int kind;
    int len;
    char name[20];
} Msg;

char *get_value(char *path, char *key, char *const ans);
int socket_create(int port);
int socket_();
int shake_hand(struct sockaddr_in host, char *name);
int find_min(int *sum, int ins);
bool check_online(LinkedList *head, struct sockaddr_in client, int ins); 
void accessible(LinkedList head, char *name);
int shake_try(struct sockaddr_in host, char *);
void list_add(LinkedList p, struct sockaddr_in *list, int *ind);
int shake_echg(struct sockaddr_in host, char *name, struct sockaddr_in *list, int list_len);

#endif
