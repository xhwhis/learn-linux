#include "head.h"
#include "common.h"
#include "linklist.h"

#define P(frm, color, arg...) { \
    printf("\033[;%dm" frm "\033[0m", color, ##arg); \
}

char value[50] = {0};
char name[20] = {0};
int port, ins;
char path[] = "./wechat.conf";

void *print(void *arg) {
    while (1) {
        LinkedList p = (LinkedList)arg;
        sleep(5);
        P("[Login] : ", 32);
        while (p->next != NULL) {
            printf("%s->%s\n", inet_ntoa(p->next->addr.sin_addr), p->next->name);
            p = p->next;
        }
    }
    return NULL;
}

int main() {
    get_value(path, "name", name);    
    port = atoi(get_value(path, "listenport", value));
    ins = atoi(get_value(path, "ins", value));    
    unsigned int s, f;
    s = ntohl(inet_addr(get_value(path, "startip", value))); 
    f = ntohl(inet_addr(get_value(path, "endip", value)));

    LinkedList *linkedlist = (LinkedList *)malloc(sizeof(LinkedList) * ins);
    int *sum = (int *)malloc(sizeof(int) * ins);
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    initaddr.sin_port = htons(port);
    
    for (int i = 0; i < ins; i++) {
        Node *node = (Node *)malloc(sizeof(Node));
        node->addr = initaddr;
        strcpy(node->name, "init");
        node->next = NULL;
        linkedlist[i] = node;
    }
    
    for (int i = s; i <= f; i++) {
        if ((i & 255) == 255 || (i << 24) == 0) continue;
        initaddr.sin_addr.s_addr = htonl(i);
        char tmp_name[20] = {0};
        strcpy(tmp_name, name);
        if (shake_hand(initaddr, tmp_name) == 0) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->next = NULL;
            newNode->addr = initaddr;
            strcpy(newNode->name, tmp_name);
            int sub = find_min(sum, ins);
            insert(linkedlist[sub], newNode);
            sum[sub]++;
        }
    }
    pthread_t work[ins];
    for (int i = 0; i < ins; i++) {
        pthread_create(&work[i], NULL, print, (void *)linkedlist[i]);
    }
    int server_listen, sockfd;

    if ((server_listen = socket_create(port)) < 0) {
        perror("socker_create()");
        return 1;
    }
    while (1) {
        P("[Listen]", 34);
        printf("Listening...\n");
        struct sockaddr_in client;
        socklen_t s_len = sizeof(client);
        fflush(stdout);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, &s_len)) < 0) {
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
                send(sockfd, name, sizeof(name), 0);
                printf("%s : %s Login\n", logname, inet_ntoa(client.sin_addr));
                if (check_online(linkedlist, client, ins)) {
                    Node *twoNode = (Node *)malloc(sizeof(Node));
                    twoNode->addr = client;
                    twoNode->addr.sin_port = htons(port);
                    twoNode->next = NULL;
                    strcpy(twoNode->name, logname);
                    int sub = find_min(sum, ins);
                    insert(linkedlist[sub], twoNode);
                    sum[sub]++;
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
