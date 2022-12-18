#include "menu.h"
#include "core.h"
#include <stdio.h>

const char *SAVE_FILE_NAME = "../logos/SAVE/SAVE.logo";
const char *QUIT_FILE_NAME = "../logos/QUIT/QUIT.logo";
const char *TITLE_FILE_NAME = "../logos/LOST/LOST.logo";
const char *PAUSE_FILE_NAME = "../logos/PAUSE/PAUSE.logo";
const char *NEWGAME_FILE_NAME = "../logos/NEWGAME/NEWGAME.logo";
const char *CONTINUE_FILE_NAME = "../logos/CONTINUE/CONTINUE.logo";

enum MENUSIZE {
    Y_MENU_HEIGHT = MIN_TERM_Y,
    X_MENU_WIDTH = MIN_TERM_X,
    BORDER = 1
};


int prettybar(const char *btns_names[]) {
    log_info("%s function started...", __PRETTY_FUNCTION__ );

    // Detecting terminal screen size
    int term_y = LINES;
    int term_x = COLS;
    log_info("Terminal size detected: %dx%d.", term_x, term_y);

    // Generating menu offsets
    // This way the menu will not be stretched over the entire terminal screen
    int menu_offset_y = (term_y - Y_MENU_HEIGHT) / 2;
    int menu_offset_x = (term_x - X_MENU_WIDTH) / 2;
    log_info("Menu offsets generated: %dx%d.", menu_offset_x, menu_offset_y);

    // Generating container for all menu elements
    WINDOW *menu_container = derwin(stdscr, Y_MENU_HEIGHT, X_MENU_WIDTH, menu_offset_y, menu_offset_x);
    log_trace("Container for menu creating status: %s.", ((menu_container) ? ("SUCSESS") : ("FAIL")));
    wbkgd(menu_container, ' ');
    box(menu_container, 0, 0);

    // Generating title size
    char title_text[PATH_MAX][PATH_MAX] = {};
    int text_height_y = 0;
    int text_width_x = 0;

    FILE *fp = fopen(TITLE_FILE_NAME, "r");
    if (fp == NULL) {
        log_warn("LOGO file openning status: FAIL.");
    } else {
        log_trace("LOGO file openning status: SUCCESS.");

        int columns = 0;
        int lines = 0;
        int pos = 0;
        char ch;

        while ((ch = (char)fgetc(fp)) != EOF) {
            if (ch == '\n') {
                title_text[lines++][pos] = '\0';
                columns = pos;
                pos = 0;
            } else {
                title_text[lines][pos++] = (char)ch;
            }
        }

        log_trace("LINES COUNT: %d COLUMNS COUNT: %d POS COUNT: %d.", lines, columns, pos);
        if (pos) {
            title_text[lines++][pos] = '\0';
            columns = pos;

            log_trace("NO NEW LINE IN THE END DETECTED. LINES COUNT: %d COLUMNS COUNT: %d POS COUNT: %d.", lines, columns, pos);
        }

        text_height_y = lines;
        text_width_x = columns;
        int close_status = fclose(fp);
        log_info("An array with the name has been formed: %dx%d.", text_width_x, text_height_y);
        log_trace("LOGO file closing status: %s.", ((close_status) ? ("FAIL") : ("SUCCESS")));
    }

    // Generating title offsets
    // It should be at the top in the center of the menu
    int title_offset_y = 1;
    int title_offset_x = (X_MENU_WIDTH - text_width_x) / 2;
    log_info("Title offsets generated: %dx%d.", title_offset_x, title_offset_y);

    // Generating title
    if (!text_height_y || !text_width_x) {
        log_fatal("Text from file did not detected.");
    } else {
        WINDOW *title = derwin(menu_container,
                               text_height_y + BORDER * 2, text_width_x + BORDER * 2,
                               title_offset_y, title_offset_x);
        log_trace("Title creating status: %s.", ((title) ? ("SUCCESS") : ("FAIL")));
        box(title, 0, 0);

        // Inserting title text
        log_trace("LOGO printing started...");
        for (int cur_line = 0; cur_line < text_height_y; cur_line++) {
            mvwprintw(title, BORDER + cur_line, 1, "%s", title_text[cur_line]);
        }

        log_trace("LOGO printing ended.");
    }

    int refresh_status = refresh();
    log_trace("Terminal window refreshing status: %s.", ((refresh_status) ? ("FAIL") : ("SUCSESS")));

//    keypad(menu_container, TRUE);
//    char ch = 0;
//    while (1) {
//        ch = wgetch(menu_container);
//
//        switch (ch) {
//            case 'q':
//                goto input_end;
//                break;
//            default:
//                box(menu_container, ch, ch);
//                refresh_status = refresh();
//                log_trace("Terminal window refreshing status: %s.", ((refresh_status) ? ("FAIL") : ("SUCSESS")));
//
//                break;
//        }
//    }
//
//    input_end:


//    int refresh_status = refresh();
//    log_trace("Terminal window refreshing status: %s.", ((refresh_status) ? ("FAIL") : ("SUCSESS")));
    log_info("%s function ended.", __PRETTY_FUNCTION__);
}
