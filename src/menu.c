#include "menu.h"
#include "core.h"
#include <stdio.h>

//enum PRETTYBAR {ENTIRE_BAR_Y = MIN_TERM_Y,
//                ENTIRE_BAR_X = MIN_TERM_X,
//                BORDER = 1,
//                BORDERS = BORDER * 2,
//                TITLE_Y = 8,
//                TITLE_X = ENTIRE_BAR_X,
//                TITLE_X_TXT = 42,
//                TITLE_X_INDENT = 1,
//                TITLE_Y_INDENT = 1,
//                BTN_SIZE_Y = 5,
//                BTN_SIZE_X = 48,
//                SPACE = 1,
//                BTN_NUM = 3};
//
//
//int prettybar(const char *btns_names[]) {
//    log_info("%s function entered", __PRETTY_FUNCTION__);
//    // Variables with size of current terminal window
//    int scr_y = LINES;
//    int scr_x = COLS;
//
//    getmaxyx(stdscr, scr_y, scr_x);
//    log_debug("Y screen size is: %d\t\tX screen size is: %d", scr_y, scr_x);
//
//    // Indents for menu bar
//    int indent_x = (scr_x - ENTIRE_BAR_X) / 2;
//    int indent_y = (scr_y - ENTIRE_BAR_Y) / 2;
//    log_debug("INDENT_Y_FOR_CONTAINER is: %d\t\tINDENT_X_FOR_CONTAINER is: %d", indent_y, indent_x);
//
//    // Container for all menu bar elements located in the center
//    WINDOW *container = derwin(stdscr, ENTIRE_BAR_Y, ENTIRE_BAR_X, indent_y, indent_x);
//    log_trace("CONTAINER create status: %d", ((container) ? (TRUE) : (FALSE)));
//    wbkgd(container, ' ');
//    box(container, 0, 0);
//
//    // TODO: Adjust TITLE_X
//    WINDOW *title = derwin(container, TITLE_Y, TITLE_X - BORDERS, TITLE_Y_INDENT, TITLE_X_INDENT);
//    log_trace("TITLE create status: %d", ((title) ? (TRUE) : (FALSE)));
//    log_trace("TITLE created with parameters: %d %d %d %d", TITLE_Y - BORDERS, TITLE_X - BORDERS,
//            TITLE_Y_INDENT, TITLE_X_INDENT);
//    wbkgd(title, ':');
//    //box(title, 0, 0);
//
//    // TODO: Remove hardcode name
//    FILE *fp = fopen("LOST.logo", "r");
//
//    if (fp == NULL) {
//        // TODO: fallback logo
//        log_trace("Failed to open file with title, skipping. Title will remain empty");
//
//    } else {
//        log_trace("File opened. Reading into title");
//        int title_indent_x = ((getmaxx(title) - TITLE_X_TXT) / 2);
//        log_trace("indent of title text is: %d", title_indent_x);
//
//        char buf[PATH_MAX] = {};
//        int pos = 0;
//        int line = 0; // initial offset is 1 border width
//        int ch;
//
//        while ((ch = fgetc(fp)) != EOF) {
//            if (ch == '\n') {
//                buf[pos] = '\0';
//                pos = 0;
//
//                // TODO: redefine BORDER to INT = 1
//                mvwprintw(title, BORDER * (line++), title_indent_x, "%s", buf);
//                log_trace("Printing to title [line #%d] string:\n%s", line - 1, buf);
//            } else {
//                buf[pos++] = (char)ch;
//            }
//        }
//
//        log_trace("writing done. closing file");
//        fclose(fp);
//        log_trace("file operation done");
//    }
//    log_trace("title should be filled now");
//
//    WINDOW *btns[BTN_NUM] = {};
//
//    // Buttons building
//    int btn_indent_x = (ENTIRE_BAR_X - (BTN_SIZE_X + BORDERS)) / 2;
//    log_trace("BUTTON INDENT X is: %d", btn_indent_x);
//
//    for (int i = 0; i < BTN_NUM; i++) {
//        btns[i] = derwin(
//                container,
//                BTN_SIZE_Y + BORDERS,
//                BTN_SIZE_X + BORDERS,
//                TITLE_Y + SPACE + (BTN_SIZE_Y + BORDERS + SPACE) * i,
//                btn_indent_x
//        );
//
//        log_trace("DERWIN returned: %d", errno);
//        log_trace("Creating BUTTON #%d: %d %d %d %d", i, BTN_SIZE_Y + BORDERS, BTN_SIZE_X + BORDERS,
//                  TITLE_Y + BORDERS + SPACE + (BTN_SIZE_Y + BORDERS + SPACE) * i, btn_indent_x);
//        log_trace("BUTTON #%d created: %d", i, ((btns[i]) ? (TRUE) : (FALSE)));
//
//        wbkgd(container, ' ');
//        box(btns[i], 0, 0);
//    }
//
//    refresh();
//    log_trace("Window refreshed.");
//    log_info("%s function exited", __PRETTY_FUNCTION__);
//}

const char *SAVE_FILE_NAME = "../logos/SAVE.logo";
const char *QUIT_FILE_NAME = "../logos/QUIT.logo";
const char *TITLE_FILE_NAME = "../logos/LOST.logo";
const char *PAUSE_FILE_NAME = "../logos/PAUSE.logo";
const char *NEWGAME_FILE_NAME = "../logos/NEWGAME.logo";
const char *CONTINUE_FILE_NAME = "../logos/CONTINUE.logo";

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

        // TODO: Why [+1]?
        text_height_y = lines + 1;
        text_width_x = columns + 1;
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
                               text_height_y + BORDER, text_width_x + BORDER,
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
    log_info("%s function ended.", __PRETTY_FUNCTION__);
}


int * prettybtn(const char *btn_name) {

}