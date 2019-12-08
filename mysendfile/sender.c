#include "head.h"

struct FileMsg{
    int size;
    char name[20];
    char buff[512];
};

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage:./sender ip port\n");
        exit(1);
    }
    int sockfd, port, size = 0;
    char buff[100] = {0}, filename[50] = {0};
    port = atoi(argv[2]);
    while (1) {
        scanf("%[^\n]s", buff);
        getchar();
        if (!strncmp("send ", buff, 5)) {
            strcpy(filename, buff + 5);
        } else {
            printf("invalid commands!\n");
            continue;
        }
        FILE *fp = NULL;
        if ((fp = fopen(filename, "rb")) == NULL) {
            printf("Error Filename!\n");
            continue;
        }
        fseek(fp, 0L, SEEK_END);
        struct FileMsg filemsg;
        filemsg.size = ftell(fp);
        strcpy(filemsg.name, filename);
        printf("Before connect!\n");
        if ((sockfd = socket_connect_ip(port, argv[1])) < 0 ){
            perror("connect");
            exit(1);
        }
        printf("After connect!\n");
        fseek(fp, 0L, SEEK_SET);
        while ((size = fread(filemsg.buff, 1, 512, fp))) {
            send(sockfd, (char *)&filemsg, sizeof(filemsg), 0);
            memset(filemsg.buff, 0, sizeof(filemsg.buff));
            printf("%d %d\n",size, filemsg.size);
        }
        close(sockfd);
        printf("After send!\n");
    }
    return 0;
}
