#include "head.h"
#ifndef _COMMON_H
#define _COMMON_H

int socket_create(int port);
int socket_connect(int port, char *host);
int get_value(char *pathname, char* key_name, char *value);
bool connect_nonblock(int port, char *host, long timeout);

#endif
