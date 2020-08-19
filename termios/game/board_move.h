#ifndef _BOARD_MOVE_H
#define _BOARD_MOVE_H

#include "common.h"

void move_left() {
    board_left--, board_right--;
    gotoxy_putc(board_left, width - 1, board);
    gotoxy_putc(board_right + 1, width - 1, blank);
}

void move_right() {
    board_left++, board_right++;
    gotoxy_putc(board_right, width - 1, board);
    gotoxy_putc(board_left - 1, width - 1, blank);
}

void board_path(){
    int choice;
    while (1) {
        choice = getch();
        if (choice == KEY_LEFT) {
            if (board_left > 1) move_left();
        } else if (choice == KEY_RIGHT) {
            if (board_right < length) move_right();
        }
    }
}

/*void board_slower() {
    msec += _msec;
    ticker(msec);
    board_path();
    refresh();
}

void board_faster() {
    msec -= _msec;
    ticker(msec);
    board_path();
    refresh();
}*/

void board_move() {
    /*int cmd;
    while (1) {
        cmd = getch();
        if (cmd == 'f') {
            board_faster();
        } else if (cmd == 's') {
            board_slower();
        }
    }*/
    board_path();
}

#endif
