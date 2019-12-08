#include "head.h"

struct Msg *msg;

int main() {
    int shmid;
    msg = (struct Msg *)get_shm("/home/lws/workspace/v4/", 8731, &shmid);
    if (msg == NULL) {
        exit(1);
    }
    while (1) {
        pthread_mutex_lock(&msg->mutex);
        printf ("<INPUT>: ");
        scanf ("%s", msg->message);
        pthread_mutex_unlock(&msg->mutex);
        pthread_cond_signal(&msg->cond);
        usleep(20);
    }
    return 0;
}
