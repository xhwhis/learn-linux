#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

char *share_memory = NULL;

void print() {
    printf("<Father> : %s\n", share_memory);
}

int main() {
    int shmid;
    pid_t pid;
    key_t key = ftok(".", 8731);
    if ((shmid = shmget(key, 4096, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((share_memory = shmat(shmid, NULL, 0)) < 0) {
        perror("shmat");
        exit(1);
    }
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        while(1) {
            printf("<child> :");
            scanf("%s", share_memory);
            kill(getppid(), 10);
        }
    } else {
        while (1) {
            signal(10, print);
            pause();
        }
    }
    return 0;
}
