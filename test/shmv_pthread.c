#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t lock;
sem_t notfull, notempty;
char str[1024];

void *put() {
    while (1) {
        sem_wait(&notfull);
        pthread_mutex_lock(&lock);
        printf ("IN -> ");
        scanf ("%[^\n]s", str);
        getchar();
        sem_post(&notempty);
        pthread_mutex_unlock(&lock);
    }
}

void *get() {
    while (1) {
        sem_wait(&notempty);
        pthread_mutex_lock(&lock);
        printf ("OUT <- %s\n", str);
        sem_post(&notfull);
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&notfull, 0, 1);
    sem_init(&notempty, 0, 0);
    pthread_t thA, thB;
    pthread_create(&thA, NULL, put, NULL);
    pthread_create(&thB, NULL, get, NULL);
    pthread_join(thA, NULL);
    pthread_join(thB, NULL);
    return 0;
}
