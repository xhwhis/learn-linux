#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

struct Msg {
    char message[64];
    pthread_condattr_t c_attr;
    pthread_cond_t cond;
    pthread_mutexattr_t m_attr;
    pthread_mutex_t mutex;
};

void *get_shm(char *path, int id, int *shmid) {
    char *share_memory = NULL;
    key_t key = ftok(path, id);
    if ((*shmid = shmget(key, sizeof(struct Msg), IPC_CREAT | 0600)) < 0) {
        perror("shmget");
    }
    if ((share_memory = shmat(*shmid, NULL, 0)) == NULL) {
        perror("shmat");
    }
    return share_memory;
}
