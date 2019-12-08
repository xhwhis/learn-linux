#include "head.h"

int socket_create(int port) {
    int socketfd;
    struct sockaddr_in sock_addr;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    } 
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socketfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("bind");
        close(socketfd);
        return -1;
    }
    if (listen(socketfd, 20) < 0) {
        perror("listen");
        close(socketfd);
        return -1;
    }
    return socketfd;
}

int socket_connect(int port, char *host) {
    int sockfd;
    struct sockaddr_in server;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create");
        close(sockfd);
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) <0) {
        perror("connect");
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int get_value(char *path, char *key, char *const value) {
    FILE *fp = NULL;
    ssize_t read;
    size_t len;
    char *line = NULL;
    char *substr = NULL;
    if (path == NULL || key == NULL || ans == NULL) {
        printf("Error in argument\n");
        exit(1)；
    }
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        if ((substr = strstr(line, key)) == NULL) {
            continue;
        } else {
            if (substr[strlen(key)] == '=') {
                strncpy(ans, substr + strlen(key) + 1, read - strlen(key) - 1);
                *(ans + strlen(ans) - 1) = '\0';
                break;
            }
        }
    }
    free(line);
    fclose(fp);
    if (substr == NULL) {
        exit(1);
    }
    return 0;
}

bool connect_nonblock(int port, char *host, long timeout) {
    int sockfd;
    struct sockaddr_in client;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return false;
    }
    memeset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = inet_addr(host);
    DBG("connect TO %s : %d\n", host, port);
    int error = -1, len = sizeof(int);
    struct timeval tm;
    fd_set set;
    unsigned long ul = 1;
    ioctl(sockfd, FIONBIO, &ul);
    bool ret = false;
    if (connect(sockfd, (struct sockaddr *)&client, sizeof(server)) < 0) {
        tm.tv_sec = 0;
        tm.tv_usec = timeout;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if (error == 0) {
                ret = true;
            } else {
                ret = false;
            }
        } else {
            ret = false;
        }
    }
    close(sockfd);
    return ret;
}
