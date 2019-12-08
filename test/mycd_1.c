#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define max_n 100

int main(int argc, char *argv[]) {
    char path[max_n] = {0};
    if (argc > 2) {
        printf ("Invalib Argument\n");
        return 1;
    } else if (argc == 1) {
        strcpy(path, ".");
    } else {
        strcpy(path, argv[1]);
    }
    printf ("Before: %s\n", getcwd(NULL, 100));
    if (chdir(path) < 0) {
        printf("Path Error: %s\n", strerror(errno));
        return 2;
    }
    printf ("After: %s\n", getcwd(NULL, 100));
    return 0;
}
