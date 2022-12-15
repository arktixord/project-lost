#include "menu.h"
#include "core.h"
#include <stdio.h>

enum PRETTYBAR {ENTIRE_BAR_Y = MIN_TERM_Y,
                ENTIRE_BAR_X = MIN_TERM_X,
                BORDER = 1,
                BORDERS = BORDER * 2,
                TITLE_Y = 8,
                TITLE_X = ENTIRE_BAR_X,
                TITLE_X_TXT = 42,
                TITLE_X_INDENT = 1,
                TITLE_Y_INDENT = 1,
                BTN_SIZE_Y = 5,
                BTN_SIZE_X = 48,
                SPACE = 1,
                BTN_NUM = 3};


int prettybar(const char *btns_names[]) {
    log_info("%s function entered", __PRETTY_FUNCTION__);
    // Variables with size of current terminal window
    int scr_y = LINES;
    int scr_x = COLS;

    getmaxyx(stdscr, scr_y, scr_x);
    log_debug("Y screen size is: %d\t\tX screen size is: %d", scr_y, scr_x);

    // Indents for menu bar
    int indent_x = (scr_x - ENTIRE_BAR_X) / 2;
    int indent_y = (scr_y - ENTIRE_BAR_Y) / 2;
    log_debug("INDENT_Y_FOR_CONTAINER is: %d\t\tINDENT_X_FOR_CONTAINER is: %d", indent_y, indent_x);

    // Container for all menu bar elements located in the center
    WINDOW *container = derwin(stdscr, ENTIRE_BAR_Y, ENTIRE_BAR_X, indent_y, indent_x);
    log_trace("CONTAINER create status: %d", ((container) ? (TRUE) : (FALSE)));
    wbkgd(container, ' ');
    box(container, 0, 0);

    // TODO: Adjust TITLE_X
    WINDOW *title = derwin(container, TITLE_Y, TITLE_X - BORDERS, TITLE_Y_INDENT, TITLE_X_INDENT);
    log_trace("TITLE create status: %d", ((title) ? (TRUE) : (FALSE)));
    log_trace("TITLE created with parameters: %d %d %d %d", TITLE_Y - BORDERS, TITLE_X - BORDERS,
            TITLE_Y_INDENT, TITLE_X_INDENT);
    wbkgd(title, ':');
    //box(title, 0, 0);

    // TODO: Remove hardcode name
    FILE *fp = fopen("LOST.logo", "r");

    if (fp == NULL) {
        // TODO: fallback logo
        log_trace("Failed to open file with title, skipping. Title will remain empty");

    } else {
        log_trace("File opened. Reading into title");
        int title_indent_x = ((getmaxx(title) - TITLE_X_TXT) / 2);
        log_trace("indent of title text is: %d", title_indent_x);

        char buf[PATH_MAX] = {};
        int pos = 0;
        int line = 0; // initial offset is 1 border width
        int ch;

        while ((ch = fgetc(fp)) != EOF) {
            if (ch == '\n') {
                buf[pos] = '\0';
                pos = 0;

                // TODO: redefine BORDER to INT = 1
                mvwprintw(title, BORDER * (line++), title_indent_x, "%s", buf);
                log_trace("Printing to title [line #%d] string:\n%s", line - 1, buf);
            } else {
                buf[pos++] = (char)ch;
            }
        }

        log_trace("writing done. closing file");
        fclose(fp);
        log_trace("file operation done");
    }
    log_trace("title should be filled now");

    WINDOW *btns[BTN_NUM] = {};

    // Buttons building
    int btn_indent_x = (ENTIRE_BAR_X - (BTN_SIZE_X + BORDERS)) / 2;
    log_trace("BUTTON INDENT X is: %d", btn_indent_x);

    for (int i = 0; i < BTN_NUM; i++) {
        btns[i] = derwin(
                container,
                BTN_SIZE_Y + BORDERS,
                BTN_SIZE_X + BORDERS,
                TITLE_Y + SPACE + (BTN_SIZE_Y + BORDERS + SPACE) * i,
                btn_indent_x
        );

        log_trace("DERWIN returned: %d", errno);
        log_trace("Creating BUTTON #%d: %d %d %d %d", i, BTN_SIZE_Y + BORDERS, BTN_SIZE_X + BORDERS,
                  TITLE_Y + BORDERS + SPACE + (BTN_SIZE_Y + BORDERS + SPACE) * i, btn_indent_x);
        log_trace("BUTTON #%d created: %d", i, ((btns[i]) ? (TRUE) : (FALSE)));

        wbkgd(container, ' ');
        box(btns[i], 0, 0);
    }

    refresh();
    log_trace("Window refreshed.");
    log_info("%s function exited", __PRETTY_FUNCTION__);
}
