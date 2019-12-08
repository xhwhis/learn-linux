#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char filename[100] = {0}, name[100] = {0}, tmp[10] = {0}, flag[10] = {0};
    if (argc != 2) {
        printf ("Usage: ./a.out codefile\n");
        exit(0);
    }
    strcpy(filename, argv[1]);
    char *sub;
    sub = strstr(filename, ".");
    strncpy(name, filename, sub - filename);
    strcpy(tmp, sub);
    printf ("File Name is %s\n", name);
    printf ("File Type is %s\n", tmp);
    if (strcmp(tmp, ".c") == 0) {
        strcpy(flag, "gcc");
    } else if (strcmp(tmp, ".cpp") == 0) {
        strcpy(flag, "g++");
    } else {
        printf ("Not Support File Type\n");
        exit(1);
    }
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror ("fork");
        exit(2);
    }
    if (pid == 0) {
        execlp("/usr/bin/vim", "vim", filename, NULL);
    }
    wait(NULL);
    if ((pid = fork()) < 0) {
        perror ("fork");
        exit(3);
    }
    if (pid == 0) {
        if (strcmp(flag, "gcc") == 0) {
            execlp("/usr/bin/gcc", "gcc", filename, "-o", name, NULL);
        } else {
            execlp("/usr/bin/g++", "g++", filename, "-o", name, NULL);
        }
    }
    wait(NULL);
    if ((pid = fork()) < 0) {
        perror ("fork");
        exit(4);
    }
    if (pid == 0) {
        char bin[100] = "./";
        strcat(bin, name);
        execlp(bin, name, NULL);
    }
    wait(NULL);
    return 0;
}
