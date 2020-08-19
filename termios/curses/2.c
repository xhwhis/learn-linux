#include <curses.h>
#include <unistd.h>

int main() {
    initscr();
    clear();
    for (int i = 0; i < LINES; i++) {
        move(i, i + 1);
        if (i % 2 == 1) {
            standout();
        }
        addstr("Hello World!");
        if (i % 2 == 1) {
            standend();
        }
        sleep(1);
        refresh();
        move(i - 1, i);
        addstr("            ");
        move(i, i + 1);
        refresh();
    }
    refresh();
    getch();
    endwin();
    return 0;
}
