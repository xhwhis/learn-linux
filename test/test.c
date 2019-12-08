#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    char *str = "abcd";
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        printf ("child: %d -> %d\n", getpid(), getppid());
    } else {
        printf ("father: %d\n", getpid());
        sleep(2);
    }
    printf ("%s\n", str);
    return 0;
}
