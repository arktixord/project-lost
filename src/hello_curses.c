#include "hello_curses.h"

void init() {
    WINDOW *root_scr = initscr();
    noecho();
    cbreak();

    box(root_scr, 0, 0);

    endwin();
}