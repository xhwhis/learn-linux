#include "common.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./server port\n");
        exit(1);
    }
    int server_listen, sockfd, port;
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        exit(2);
    }
    while (1) {
        struct sockaddr_in client_addr;
        int len = sizeof(client_addr);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client_addr, (socklen_t *)&len)) < 0) {
            perror("accept");
            continue;
        }
        char buff[100] = {0}, filename[20] = {0};
        Message msg;
        ssize_t len = 0;
        len += recv(sockfd, &msg, 512, 0);
        strcpy(filename, msg.name);
        strcpy(buff, msg.buff);
        printf("msg.name= %s\n", msg.name);
        printf("msg.buff= %s\n", msg.buff);
        strcat(filename, ".server");
        FILE * pfile = fopen(filename, "a");
        fwrite(buff, 1, 512, pfile);
        memset(buff, 0, sizeof(buff));
        while(len < msg.size) {
            len += recv(sockfd, &msg, 512, 0);
            strcpy(filename, msg.name);
            strcpy(filename, msg.buff);
            fwrite(buff, 1, 512, pfile);
            memset(buff, 0, sizeof(buff));
        }
        close(sockfd);
    } 
    return 0;
}
