#include "common.h"

int main(int argc, char** argv) {
    int sockfd;
    char buff[512];
    char *server = argv[1];
    int port = atoi(argv[2]);
    char *name = argv[3];
    struct sockaddr_in server_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        close(sockfd);
        exit(1);
    }
    send(sockfd, name, strlen(name), 0);
    while (1) {
        scanf("%[^\n]s", buff);
        getchar();
        send(sockfd, buff, strlen(buff), 0);
    }
    close(sockfd);
    return 0;
}
