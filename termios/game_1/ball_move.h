#ifndef _BALL_MOVE_H
#define _BALL_MOVE_H

#include "common.h"

void ball_move() {
    signal(SIGALRM, ball_move);
    gotoxy_putc(ball_x, ball_y, blank);
    ball_x += row_path, ball_y += col_path;
    if (ball_x >= length - 1) row_path = -1;
    if (ball_x <= 2) row_path = 1;
    if (ball_y >= width - 1) col_path = -1;
    if (ball_y <= 1) {
        col_path = 1;
    }
    gotoxy_putc(ball_x, ball_y, ball);
}

#endif
