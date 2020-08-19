#ifndef _BALL_MOVE_H
#define _BALL_MOVE_H

#include "common.h"

/*void move_left() {
    ball_x--;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x + 1, ball_y, blank);
}

void move_right() {
    ball_x++;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x - 1, ball_y, blank);
}

void move_up() {
    ball_y--;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x, ball_y + 1, blank);
}

void move_down() {
    ball_y++;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x, ball_y - 1, blank);
}

void move_leftup() {
    ball_x--, ball_y--;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x + 1, ball_y + 1, blank);
}

void move_leftdown() {
    ball_x--, ball_y++;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x + 1, ball_y - 1, blank);
}

void move_rightup() {
    ball_x++, ball_y--;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x - 1, ball_y + 1, blank);
}

void move_rightdown() {
    ball_x++, ball_y++;
    gotoxy_putc(ball_x, ball_y, ball);
    gotoxy_putc(ball_x - 1, ball_y - 1, blank);
}

void choose_path(){
    int choice;
    while (1) {
        choice = getch();
        if (choice == KEY_LEFT) {
            move_left();
        } else if (choice == KEY_RIGHT) {
            move_right();
        } else if (choice == KEY_UP) {
            move_up();
        } else if (choice == KEY_DOWN) {
            move_down();
        }
    }
}

void move_slower() {
    msec += _msec;
    ticker(msec);
    choose_path();
    refresh();
}

void move_faster() {
    msec -= _msec;
    ticker(msec);
    choose_path();
    refresh();
}*/

void ball_move() {
    /*int cmd;
    cmd = getch();
    while (1) {
        if (cmd == 'f') {
            move_faster();
        } else if (cmd == 's') {
            move_slower();
        }
    }*/
    signal(SIGALRM, ball_move);
    gotoxy_putc(ball_x, ball_y, blank);
    ball_x += row_path, ball_y += col_path;
    if (ball_x = length) row_path = -1;
    if (ball_x = 0) row_path = 1;
    if (ball_y = width - 1) col_path = -1;
    if (ball_y = 0) {
        col_path = 1;
    }
    gotoxy_putc(ball_x, ball_y, ball);
}

#endif
