#ifndef _BOARD_MOVE_H
#define _BOARD_MOVE_H

#include "common.h"

void move_left() {
    board_left--, board_right--;
    gotoxy_putc(board_left, width, board);
    gotoxy_putc(board_right + 1, width, blank);
}

void move_right() {
    board_left++, board_right++;
    gotoxy_putc(board_right, width, board);
    gotoxy_putc(board_left - 1, width, blank);
}

void board_move() {
    int cmd;
    while (1) {
        cmd = getch();
        if (cmd == KEY_LEFT) {
            if (board_left > 1) move_left();
        } else if (cmd == KEY_RIGHT) {
            if (board_right < length) move_right();
        }
    }
}

#endif
