#include "space.h"
#include "ball_move.h"
#include "board_move.h"

int main() {
    initspace();
    signal(SIGALRM, ball_move);
    ticker(msec);
    gotoxy_putc(ball_x, ball_y, ball);
    while (1) {
        usleep(10000);
    }
    board_move();
    return 0;
}
