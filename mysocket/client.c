#include "common.h"

int sendFile(int sockfd, char *buff) {
    char file[512] = {0};
    if (buff[0] == '#') {
        strncpy(file, buff + 1, strlen(buff) - 1);
    }
    char file_path[512] = {0};
    strcat(file_path, "./");
    strcat(file_path, file);
    FILE *fq = NULL;
    if ((fq = fopen(file_path, "rb")) == NULL) {
        perror("open");
        exit(1);
    }
    char *file_send = (char *)calloc(512, sizeof(char));
    int len;
    while (!feof(fq)) {
        len = fread(file_send, 1, 512, fq);
        send(sockfd, file_send, len, 0);
    }
    fclose(fq);
    return 0;
}

int main(int argc, char** argv) {
    int sockfd;
    char buff[512] = {0}, recvbuff[512] = {0};
    char *server = argv[1];
    int port = atoi(argv[2]);
    char *name = argv[3];
    struct sockaddr_in server_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }
    printf ("%s成功加入群聊！\n", name);
    send(sockfd, name, strlen(name), 0);
    while (1) {
        scanf("%[^\n]s", buff);
        getchar();
        if (buff[0] == '#') {
            sendFile(sockfd, buff);
        } else {
            send(sockfd, buff, strlen(buff), 0);
        }
    }
    close(sockfd);
    return 0;
}
