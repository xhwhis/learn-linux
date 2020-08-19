#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#define ASK "Do you want another try"
#define BEEP putchar('\a')

int get_ok() {
    int c;
    while ((c = getchar()) != EOF && strchr("YyNn", c) == NULL);
    return c;
}

void set_nonblock_mode() {
    int termflags;
    termflags = fcntl(0, F_GETFL);
    termflags |= O_NONBLOCK;
    fcntl(0, F_SETFL, termflags);
}

int get_response(char *ask) {
    printf ("%s (y/n)\n", ask);
    int input;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int ret = select(1, &set, NULL, NULL, &tv);
    if (ret == 1) {
        while (1) {
            input = tolower(get_ok());
            if (input == 'y') {
                return 0;
            } else if (input == 'n') {
                return 1;
            }
        }
    }
    BEEP;
    return -1;
}

void set_mode() {
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how) {
    static struct termios orginal_mode;
    static int orginal_flags;
    if (how == 0) {
        tcgetattr(0, &orginal_mode);
        orginal_flags = fcntl(0, F_GETFL);
    } else {
        tcsetattr(0, TCSANOW, &orginal_mode);
        fcntl(0, F_SETFL, &orginal_flags);
    }
}

int main() {
    int response;
    tty_mode(0);
    set_mode();
    set_nonblock_mode();
    response = get_response(ASK);
    printf("\nresponse = %d\n", response);
    tty_mode(1);
    return 0;
}
