#include "battle_mode.h"

void init() {
    initscr();
    noecho();
    cbreak();
}

void deinit() {
    nocbreak();
    echo();
    endwin();
}
