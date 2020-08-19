#include <ncurses.h>

int main() {
    initscr();
    move(10, 50);
    addstr("Hello World\n");
    refresh();
    getch();
    endwin();
    return 0;
}
