#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    char cmd[100];
    char *args[100];
    while (1) {
        printf("lws#: ");
        fflush(stdin);
        fgets(cmd, 100, stdin);
        int i = 0;
        while (cmd[i] != '\n') {
            i++;
        }
        cmd[i] = '\0'; 
        args[0] = cmd;
        for (i = 0; *args[i]; i++) {
            for (args[i+1] = args[i] + 1; *args[i+1]; args[i+1]++) {
                if (*args[i+1] == ' ') {
                    *args[i+1] = '\0';
                    args[i+1]++;
                    break;
                }
            }
        }
        args[i] = NULL;
        if (!args[0]) continue;
        if (strcmp(args[0], "cd") == 0) {
            if (args[1]) chdir(args[1]);
            continue;
        }
        if (strcmp(args[0], "pwd") == 0) {
            char val[100];
            puts(getcwd(val, 100));
            continue;
        }
        if (strcmp(args[0], "exit") == 0) return 0;
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            return 2;
        }
        wait(NULL);
    }
}
