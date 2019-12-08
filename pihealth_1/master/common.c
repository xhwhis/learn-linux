#include "common.h"

int socket_create(int port) {
    int sockfd;
    struct sockaddr_in addr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        perror("listen");
        close(sockfd);
        return -1;
    }
    return sockfd;
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

int get_conf_value(char *pathname, char* key_name, char *value) {
    FILE *fd = NULL;
    char *line = NULL;
    char *substr = NULL;
    ssize_t read = 0;
    size_t len = 0;
    int tmp = 0;
    fd = fopen(pathname, "r");
    if (fd == NULL) {
        printf("Error in Open\n");
        exit(1);
    }
    while ((read = getline(&line, &len, fd)) != 1) {
        fflush(stdout);
        substr = strstr(line, key_name);
        if (substr == NULL) {
            continue;
        } else {
            tmp = strlen(key_name);
            if (line[tmp] == '=') {
                strncpy(value, &line[tmp + 1], (int)read - tmp - 1);
                tmp = strlen(value);
                *(value + tmp - 1) = '\0';
                break;
            }
            else {
                printf("Next\n");
                continue;
            }
        }
    }
}
