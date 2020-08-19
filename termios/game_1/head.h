#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#define length 64
#define width 32
#define board_length 8
#define _msec 1

char ball = 'o';
char board = '=';
char blank = ' ';
int ball_x = length / 2, ball_y = width - 1;
int board_left = (length - board_length) / 2;
int board_right = (length + board_length) / 2;
int row_path = 1, col_path = 1;
int msec = 100;

#endif
