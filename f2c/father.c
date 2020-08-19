#include "head.h"

struct Msg *msg;

int main() {
    int shmid;
    msg = (struct Msg *)get_shm("/home/lws/linux/f2c/", 8731, &shmid);
    if (msg == NULL) {
        exit(1);
    }
    pthread_mutexattr_init(&msg->m_attr);
    pthread_condattr_init(&msg->c_attr);
    pthread_mutexattr_setpshared(&msg->m_attr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&msg->c_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setrobust(&msg->m_attr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&msg->mutex, &msg->m_attr);
    pthread_cond_init(&msg->cond, &msg->c_attr);
    while (1) {
        pthread_mutex_lock(&msg->mutex);
        pthread_cond_wait(&msg->cond, &msg->mutex);
        printf ("%s\n", msg->message);
        pthread_mutex_unlock(&msg->mutex);
    }
    return 0;
}
