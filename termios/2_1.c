#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#define LENGTH 8

char passwd[LENGTH];

int main() {
    struct termios initsettings;
    struct termios newsettings;
    printf("input your password\n");
    tcgetattr(fileno(stdin), &initsettings);
    newsettings = initsettings;
    newsettings.c_lflag &= ~ECHO;
    tcsetattr(fileno(stdin), TCSAFLUSH, &newsettings);
    fgets(passwd, LENGTH, stdin);
    tcsetattr(fileno(stdin), TCSANOW, &initsettings);
    fprintf(stdout, "your passwd is:");
    fprintf(stdout, "%s", passwd);
    return 0;
}
