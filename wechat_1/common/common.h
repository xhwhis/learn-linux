#include "head.h"
#ifndef _COMMON_H
#define _COMMON_H

char *get_val(char *path, char *key, char * const ans);
int socket_create(int port);
int socket_();
int shake_hand(struct sockaddr_in host, char *name);
int find_min(int *sum, int ins);

#endif
