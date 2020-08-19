#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void wakeup() {
    printf("Time to wake up!\n");
    printf("Sleep another 10s.\n");
    sleep(10);
}

int main() {
    signal(14, wakeup);
    alarm(5);
    pause();
    printf("Get up!\n");
    return 0;
}
