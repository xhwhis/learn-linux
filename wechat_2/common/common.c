#include "common.h"

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

char *get_value(char *path, char *key, char * const ans) {
    FILE *fp = NULL;
    ssize_t read;
    size_t len;
    char *line = NULL;
    char *substr = NULL;

    if (path == NULL || key == NULL || ans == NULL) {
        printf("Error in argument\n");
        return NULL;
    }
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return NULL;
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
        return NULL;
    }
    return ans;
}

int socket_() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket_()");
        return -1;
    }
    return sockfd;
}

int shake_hand(struct sockaddr_in host, char *name) {
	int sockfd;
	if ((sockfd = socket_()) < 0) {
		return -1;
	}
	fd_set set;
    unsigned long len = 1;
    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    ioctl(sockfd, FIONBIO, &len);
    FD_ZERO(&set);
	FD_SET(sockfd, &set);
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len_t);
            if (error != 0) {
                close(sockfd);
                return -1;
            }
            len = 0;
            ioctl(sockfd, FIONBIO, &len);
            send(sockfd, name, strlen(name), 0);
            memset(name, 0, sizeof(name));
        	recv(sockfd, name, 20, 0);
        } else {
            close(sockfd);
            return -1;
        }
	}
    close(sockfd);
    return 0;
}

int find_min(int *sum, int ins) {
    int min = 0;
    for (int i = 0; i < ins; i++) {
        if (sum[i] < sum[min]) min = i;
    }
    return min;
}

bool check_online(LinkedList *head, struct sockaddr_in client, int ins) {
    for (int i = 0; i < ins; i++) {
        Node *p = head[i];
        while (p->next != NULL) {
            if (p->next->addr.sin_addr.s_addr == client.sin_addr.s_addr) {
                return false;
            }
            p = p->next;
        }
    }
    return true;
}
