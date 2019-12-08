#include "head.h"
#ifndef _COMMON_H
#define _COMMON_H

char *get_value(char *path, char *key, char * const ans);
int socket_create(int port);
int socket_();
int shake_hand(struct sockaddr_in host, char *name);
int find_min(int *substrm, int ins);
bool check_online(LinkedList *head, struct sockaddr_in client, int ins);

#endif
