#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t th[10];
int id[10];

void *print(void *arg) {
    int *tmp = (int *)arg;
    sleep(rand() % 10 + 1);
    printf ("<%d> : %lu %lu\n", *tmp, pthread_self(), th[*tmp]);
    return NULL;
}

int main() {
    for (int i = 0; i < 10; i++) {
        id[i] = i;
        pthread_create(&th[i], NULL, print, (void *)&id[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(th[i], NULL);
    }
    return 0;
}
