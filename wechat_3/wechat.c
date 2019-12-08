#include "head.h"
#include "common.h"
#include "linklist.h"

char value[50] = {0};
char name[20] = {0};
int port, ins;
char path[] = "./wechat.conf";

void add_usr(struct sockaddr_in *s, int *sum, LinkedList *linkedlist) {
    if (check_online(linkedlist, *s, ins)) {
        Node *new = (Node *)malloc(sizeof(Node));
        new->addr = *s;
        new->addr.sin_port = htons(port);
        new->next = NULL;
        new->heart = 0;
        if (shake_hand(*s, name) != 0) return ;
        strcpy(new->name, name);
        int sub = find_min(sum, ins);
        insert(linkedlist[sub], new);
        sum[sub++];
    }
}

void *print(void *arg) {
    while (1) {
        LinkedList p = (LinkedList)arg;
        sleep(10);
        D(YELLOW_HL([LIST])"\n");
        while (p->next != NULL) {
            printf(YELLOW_HL([User])"%s_%s\n", inet_ntoa(p->next->addr.sin_addr), p->next->name);
            p = p->next;
        }
    }
    return NULL;
}

void *echg_list(void *arg) {
    while (1) {
        sleep(20);
        LinkedList* p = (LinkedList *)arg;
        int sum = 0; 
        for (int i = 0; i < ins; i++) {
            sum += p[i]->len; 
        }
        int ind = 0;
        int list_len = sizeof(struct sockaddr_in) * sum;
        struct sockaddr_in *list = (struct sockaddr_in *)malloc(list_len);
        for (int i = 0; i < ins; i++) {
            list_add(p[i], list, &ind);
        }
        for (int i = 0; i < ins; i++) {
            LinkedList usr = p[i];
            while (usr->next != NULL) {
                shake_echg(usr->next->addr, name, list, list_len);
                usr = usr->next;
            }
        }
    }
    return NULL;
}

void *send_heart(void *arg) {
    while (1) {
        sleep(1);
        D(YELLOW(HEART)"\n");
        LinkedList* p = (LinkedList *)arg;
        for (int i = 0; i < ins; i++) {
            accessible(p[i], name);
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
    int *sum= (int *)malloc(sizeof(int) * ins);

    D(BLUE_HL([Before init])"\n");
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    initaddr.sin_port = htons(port);
    for (int i = 0; i < ins; i++) { //初始化链表头结点
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        p->len = 0;
        strcpy(p->name, "init");
        p->next = NULL;
        linkedlist[i] = p;
    }

    D(GREEN_HL([Create LinkedList!])"\n");
    for (unsigned int i = s; i <= f; i++) { //上线尝试通知局域网内用户
        if ((i & 255) == 255 || i << 24 == 0) continue;
        initaddr.sin_addr.s_addr = htonl(i); //ip地址长
        char tmp_name[20] = {0};
        strcpy(tmp_name, name);
        if (shake_hand(initaddr, tmp_name) == 0) {
            Node *new = (Node *)malloc(sizeof(Node));
            new->heart = 0;
            new->next = NULL;
            new->addr = initaddr;
            strcpy(new->name, tmp_name);
            int sub = find_min(sum, ins);
            insert(linkedlist[sub], new);
            sum[sub]++; //链表长度用户数修改
        }
    }

    D(GREEN_HL([Create work pthread!])"\n"); //工作线程:聊天信息发送
    pthread_t work[ins];
    for (int i = 0; i < ins; i++) {
        pthread_create(&work[i], NULL, print, (void *)linkedlist[i]);
    }

    
    D(GREEN_HL([Create heart pthread!])"\n"); // 发送心跳线程
    pthread_t heart;
    pthread_create(&heart, NULL, send_heart, (void *)linkedlist);

    D(GREEN_HL([Create exchange pthread!])"\n"); // 交换用户列表线程
    pthread_t echg;
    pthread_create(&echg, NULL, echg_list, (void *)linkedlist);
    
    int server_listen, sockfd;
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create()");
        return 1;
    }
    while (1) {
        struct sockaddr_in client;
        socklen_t s_len = sizeof(client);
        D(BLUE_HL(Listening...)"\n");
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
                Msg *msg = (Msg *)malloc(sizeof(Msg));
                recv(sockfd, msg, sizeof(Msg), 0);
                if (msg->kind == 1) {
                    printf(YELLOW(HEART)"%s\n", msg->name);
                    close(sockfd);
                    continue;
                } else if (msg->kind == 2) {
                    D(YELLOW(TYPE2)"\n");
                    int num = msg->len / sizeof(struct sockaddr_in);
                    struct sockaddr_in *s = (struct sockaddr_in*)malloc(msg->len);
                    int now = 0;
                    while (recv(sockfd, &s[now++], sizeof(struct sockaddr_in), 0) != 0);
                    if (now != num) {
                        close(sockfd);
                        continue;
                    }
                    for (int i = 0; i < num; i++) {
                        add_usr(&s[i], sum, linkedlist);
                        D(YELLOW_HL([LIST EXG]));
                    }
                    close(sockfd);
                    continue;
                } else if (msg->kind == 0) {
                    strcpy(logname, msg->name);
                    strcpy(msg->name, name);
                    msg->len = 0;
                    send(sockfd, msg, sizeof(Msg), 0);
                    D(GREEN_HL(Login)" %s:%s\n", logname, inet_ntoa(client.sin_addr));
                    if (check_online(linkedlist, client, ins)) {
                         Node *new = (Node *)malloc(sizeof(Node));
                         new->addr = client;
                         new->addr.sin_port = htons(port);
                         new->next = NULL;
                         new->heart = 0;
                         strcpy(new->name, logname);
                         int sub = find_min(sum, ins);
                         insert(linkedlist[sub], new);
                    }
                     close(sockfd);
                } else {
                    continue;
                }
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
