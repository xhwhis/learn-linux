#ifndef _COMMON_H
#define _COMMON_H

#include "head.h"

void gotoxy_putc(int x, int y, char ch) {
    move(y, x);
    addch(ch);
    move(LINES - 1, 1);
    refresh();
}

void gotoxy_puts(int x, int y, char *str) {
    move(y, x);
    addstr(str);
    move(LINES - 1, 1);
    refresh();
}

int ticker(int msec) {
    struct itimerval new_value;
    new_value.it_value.tv_sec = msec / 1000;
    new_value.it_value.tv_usec = msec % 1000 * 1000;
    new_value.it_interval.tv_sec = msec / 1000;
    new_value.it_interval.tv_usec = msec % 1000 * 1000;
    return setitimer(ITIMER_REAL, &new_value, NULL);
}

#endif
