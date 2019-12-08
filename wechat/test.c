#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    return sockefd;
}

int main () {
    int a1 = 192, b1 = 168, c1 = 1, d1 = 1;
    int a2 = 192, b2 = 168, c2 = 244, d2 = 244;
    for (int a = a1; a < a2; a++) {
        for (int b = b1; b < b2; b++) {
            for (int c = c1; c < c2; c++) {
                for (int d = d1; d < d2; d++) {
                    if (boom(a, b, c, d)) {
                        printf ("%d:%d:%d:%d", a, b, c, d);
                    }
                }
            }
        }
    }
    return 0;
}
