#ifndef _SPACE_H
#define _SPACE_H

#include "common.h"

void initfiled(int x, int y) {
    for (int i = 2; i <= x - 1; i++) {
        gotoxy_putc(i, 0, '_');
    }
    for (int i = 1; i <= y; i++) {
        gotoxy_putc(1, i, '|');
        gotoxy_putc(x, i, '|');
    }
}

void initspace() {
    initscr();
    clear();
    if (start_color() == ERR) {
        endwin();
        printf ("终端不支持颜色\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_YELLOW, COLOR_GREEN);
    init_pair(4, COLOR_CYAN, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_RED, COLOR_MAGENTA);
    initfiled(length, width);
    attron(COLOR_PAIR(1));
    gotoxy_putc(ball_x, ball_y, ball);
    for (int i = board_left; i <= board_right; i++) {
        gotoxy_putc(i, width, board);
    }
}

#endif
