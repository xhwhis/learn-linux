#include "head.h"

char val[50] = {0};
char name[20] = {0};
int port, ins;
char path[] = "./wechat.conf";

void *print(void *arg) {
    while (1) {
        LinkedList p = (LinkedList)arg;
        sleep(1);
        printf ("print:\n");
        while (p->next != NULL) {
            printf ("%s_%s\n", inet_ntoa(p->next->addr.sin_addr), p->next->name);
            p = p->next;
        }
    }
}

int main() {
    get_val(path, "name", name);
    port = atoi(get_val(path, "listenport", val));
    ins = atoi(get_val(path, "ins", val));
    unsigned int s, f;
    s = ntohl(inet_addr(get_val(path, "startip", val)));
    f = ntohl(inet_addr(get_val(path, "endip", val)));
    LinkedList *linkedlist = (LinkedList *)malloc(ins * sizeof(LinkedList));
    int *sum = (int *)malloc(sizeof(int) * ins);
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    init.sin_port = htons(port);
    for (int i = 0; i < ins; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        strcpy(p->name, "init");
        p->next = NULL;
        linklist[i] = p;
    }
    for (unsigned int i = s; i <= f; i++) {
        if ((i & 255) == 255 || i << 24 == 0) continue;
        initaddr.sin_addr.s_addr = htonl(i);
        char tmp_name[20] = {0};
        strcpy(tmp_name, name);
        if (shake_hand(initaddr, tmp_name) == 0) {
            Node *new = (Node *)malloc(sizeof(Node));
            new->next = NULL;
            new->adde = initaddr;
            strcpy(new->name, tmp_name);
            int sub = find_min(sum, ins);
            insert(linkedlist[sub], new);
            sum[sub]++;
        }
    }
    pthread_t work[ins];
    for (int i = 0; i < ins; i++) {
        pthread_create(&work[i], NULL, print, (void *)linkedlist[i]);
    }
    int server_listen, sockfd;
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create()");
        return 1;
    }
    while (1) {
        struct sockaddr_in client;
        socklen_t s_len = sizeof(client);
        if ((accept(server_listen, (struct sockaddr *)&client, &s_len)) < 0) {
            perror("accept()");
            close(sockfd);
            continue;
        }
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        char logname[20] = {0};
        if (select(sockfd + 1, &set, NULL, NULL, &tv) > 0) {
            if (FD_ISSET(sockfd, &set)) {
                recv(sockfd, logname, sizeof(logname), 0);
                send(sockfd, name, strlen(name), 0);
                printf ("%s : %s Login!\n", logname, inet_ntoa(client.sin_addr));
                if (check_online(linkedlist, client, ins)) {
                    Node *new = (Node *)malloc(sizeof(Node));
                    new->addr = client;
                    new->addr.sin_port = htonl(port);
                    new->next = NULL;
                    strcpy(new->name, logname);
                    int sub = find_min(sum, ins);
                    insert(linkedlist[sub], new);
                }
                close(sockfd);
            } else {
                close(sockfd);
                continue;
            }
        } else {
            close(sockfd);
        }
    }
    return 0;
}
