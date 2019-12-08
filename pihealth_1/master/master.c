#include "common.h"
#include "linklist.h"
#include "color.h"

char value[50] = {0};
int heart_port, ins, heart_port_c, ctrl_port;
char path[] = "./pihealth.conf";

struct Print_Arg {
    int num;
    LinkedList lt;
    int *sum;
};

struct Listen_Arg {
    LinkedList *lk;
    int *sum;
};

struct Send_Arg {
    LinkedList *lk;
    int *sum;
};

void *print(void *arg) {
    struct Print_Arg *arg_in = (struct Print_Arg *)arg;
    char outfile[50] = {0};
    while (1) {
        LinkedList p = arg_in->lt;
        LinkedList head = p;
        sleep(10);
        while (p->next != NULL) {
            int fd;
            p->next->addr.sin_port = htons(ctrl_port);
            if ((fd = socket_connect(p->next->addr)) < 0) {
                p->next->flag++:
                printf ("×\n");
            }
            close(fd);
            p = p->next;
        } 
    }
    return NULL;
}

void *_listen(void *arg) {
    int server_listen, sockfd;
    struct Listen_Arg *arg_in = (struct Listen_Arg *)arg;
    LinkedList *linkedlist_in = arg_in->lk;
    int *sum = arg_in->sum;
    D("["RED"Main"NONE"] Create listen socket for connecting...\n");
    if ((server_listen = socket_create(heart_port)) < 0) {
        perror("socket_create()");
    }
    while (1) {
        struct sockaddr_in client;
        socklen_t s_len = sizeof(client);
        D("["BLUE"Listen"NONE"] Listening...\n");
        fflush(stdout);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, &s_len)) < 0) {
            perror("accept()");
            close(sockfd);
            continue;
        }
        D("["L_PINK"Accept"NONE"] Accepted...\n");
        D("["L_GREEN"Login"NONE"]%s Login...\n", inet_ntoa(client.sin_addr));
        if (check_online(linkedlist_in, client, ins)) {
            Node *new = (Node *)malloc(sizeof(Node));
            new->addr = client;
            new->addr.sin_port = htons(heart_port_c);
            new->next = NULL;
            int sub = find_min(sum, ins);
            insert(linkedlist_in[sub], new);
        }
        close(sockfd);
    }
}

int main() {
    int u_sum = 0;
    D("["RED"Main"NONE"] Program starting...\n");
    heart_port = atoi(get_value(path, "HeartPortMaster", value));
    ctrl_port = atoi(get_value(path, "CtrlPortClient", value));
    ins = atoi(get_value(path, "Ins", value));
    heart_port_c = atoi(get_value(path, "HeartPortClient", value));
    printf ("heart_port = %d, heart_port_c = %d\n", heart_port, heart_port_c);
    unsigned int s, f;
    s = ntohl(inet_addr(get_value(path, "StartIp", value)));
    f = ntohl(inet_addr(get_value(path, "EndIp", value)));
    printf("heart_port = %d, heart_port_c = %d\n", heart_port, heart_port_c);
    D("["RED"Main"NONE"] Init configuration...\n");
    LinkedList *linkedlist = (LinkedList *)malloc(ins * sizeof(LinkedList));
    int *sum = (int *)malooc(ins * sizeof(int));
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    initaddr.sin_port = htons(heart_port_c);
    D("["RED"Main"NONE"] Create "L_YELLOW"%d"NONE" linkedlist for user storage...\n", ins);
    for (int i = 0; i < ins; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        strcpy(p->name, "init");
        p->next = NULL;
        linkedlist[i] = p;
    }
    struct Listen_Arg l_arg;
    l_arg.lk = linkedlist;
    l_arg.sum = sum;
    pthread_t l_tid;
    pthread_create(&l_tid, NULL, _listen, (void *)&l_arg);
    D("["RED"Main"NONE"] Check for client according to config file...\n");
    for (unsigned int i = s; i <= f; i++) {
        if ((i & 255) == 255 || i << 24 == 0) continue;
        initaddr.sin_addr.s_addr = htonl(i);
        if (shake_hand(initaddr) == 0) {
            Node *new = (Node *)malloc(sizeof(Node));
            new->next = NULL;
            new->addr = initaddr;
            int sub = find_min(sum, ins);
            insert(linkedlist[sub], new);
            sum[sub]++;
            u_sum++;
        }
    }
    D("["RED"Main"NONE"] connected to "L_YELLOW"%d"NONE" User..\n", u_sum);
    D("["RED"Main"NONE"] Create "L_YELLOW"%d"NONE" pthreads for working...\n", ins);
    pthread_t work[ins];
    for (int i = 0; i < ins; i++) {
        struct Print_Arg arg;
        arg.num = i;
        arg.lt = linkedlist[i];
        pthread_create(&work[i], print, (void *)&arg);
    }
    while(1) {
        sleep(100);
    }
    return 0;
}
