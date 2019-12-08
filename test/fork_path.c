#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    char *patharg;
    patharg = getenv("PATH");
    printf ("%s", patharg);
    if (fork() == 0) {
        execlp("./b.out", 0);
    }
    wait(NULL);
    return 0;
}
