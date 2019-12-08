#include "common.h"

char* get_val(char *path, char *key, char *ans) {
    FILE *fp = NULL；
    ssize_t read;
    size_t len;
    char *line = NULL;
    char *substr = NULL;
    if (key == NULL || ans == NULL || path == NULL) {
        printf ("Error in arguement\n");
        return NULL;
    }
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return NULL;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        if ((substr = strstr(line, key)) == NULL) continue;
        else {
            if (sybstr[strlen(key)] == '=') {
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

int socket_create(int port) {
    int sockfd;
    int yes = 1;
    struct sockaddr_in sock_addr;
    struct linger m_sLinger;
    m_sLinger.l_onoff = 1;
    m_sLinger.l_linger = 0;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return -1;
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(solckfd, SOL_SOCKET, SO_LINGER, (const char *)&m_sLinger, sizeof(struct linger));
    if ((setsockopt(sockfd, SOL_SOCKET, SO_LINGER), SO_REUSEADDR, &yes, sizeof(int)) == -1) {

    }
    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        close (sockfd);
        perror("listen() error");
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        close(sockfd);
        perror("listen() error");
        return -1;
    }
    return sockfd;
}

int shake_hand(struct sockaddr_in host, char *name) {
    int sockfd;
    if ((sockfd = socket_()) < 0) {
        return -1;
    }
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if (error != 0) {
                close(sockfd);
                return -1;
            }
            send(sockfd, name, strlen(name), 0);
            memset(name, 0, sizeof(name));
            recv(name, 0, sizeof(name), 0);
        } else {
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
    return 0;
}

int find_min(int *sum, int ins) {
    int sub = 0;
    for (int i = 0; i < ins; i++) {
        if (*(sum + i) < *(sum + sub)) {
            sub = i;
        }
    }
    return sub;
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
