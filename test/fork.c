#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int a = 0;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        printf ("child: %d -> %d\n", getpid(), getppid());
    } else {
        printf ("father: %d\n", getpid());
        sleep(2);
    }
    printf ("%d\n", ++a);
    return 0;
}
