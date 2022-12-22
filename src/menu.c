#include "menu.h"
#include "core.h"
#include <stdio.h>

const char *SAVE_FILE_NAME = "../logos/SAVE.logo";
const char *QUIT_FILE_NAME = "../logos/QUIT.logo";
const char *TITLE_FILE_NAME = "../logos/LOST.logo";
const char *PAUSE_FILE_NAME = "../logos/PAUSE.logo";
const char *NEWGAME_FILE_NAME = "../logos/NEWGAME.logo";
const char *CONTINUE_FILE_NAME = "../logos/CONTINUE.logo";

enum MENUSIZE {
    MENU_SIZE_Y = MIN_TERM_Y,
    MENU_SIZE_X = MIN_TERM_X,
    BORDER = 1
};


int prtybar(int btns_number, const char *btns_names[]) {
    log_info("%s function started...", __PRETTY_FUNCTION__ );

    // Detecting terminal screen size
    int term_y = LINES;
    int term_x = COLS;
    log_info("Terminal size detected: %dx%d.", term_x, term_y);

    // Generating menu offsets
    // This way the menu will not be stretched over the entire terminal screen
    int menu_offset_y = (term_y - MENU_SIZE_Y) / 2;
    int menu_offset_x = (term_x - MENU_SIZE_X) / 2;
    log_info("Menu offsets generated: %dx%d.", menu_offset_x, menu_offset_y);

    // Generating container for all menu elements
    WINDOW *menu_container = derwin(stdscr, MENU_SIZE_Y, MENU_SIZE_X, menu_offset_y, menu_offset_x);
    log_trace("Container for menu creating status: %s.", ((menu_container) ? ("SUCSESS") : ("FAIL")));
    wbkgd(menu_container, ' ');
    box(menu_container, 0, 0);

    // Generating title size
    char title_text[PATH_MAX][PATH_MAX] = {};
    int text_size_y = 0;
    int text_size_x = 0;

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

        if (pos) {
            title_text[lines++][pos] = '\0';
            columns = pos;

            log_trace("No empty line at the end of file detected.");
        }

        text_size_y = lines;
        text_size_x = columns;
        int close_status = fclose(fp);
        log_info("An array with the name has been formed: %dx%d.", text_size_x, text_size_y);
        log_trace("LOGO file closing status: %s.", ((close_status) ? ("FAIL") : ("SUCCESS")));
    }

    // Generating title offsets
    // It should be at the top in the center of the menu
    int title_offset_y = 1;
    int title_offset_x = (MENU_SIZE_X - text_size_x) / 2;
    log_info("Title offsets generated: %dx%d.", title_offset_x, title_offset_y);

    // Generating title
    if (!text_size_y || !text_size_x) {
        log_warn("Text from file did not detected.");
    } else {
        WINDOW *title = derwin(menu_container,
                               text_size_y + BORDER * 2, text_size_x + BORDER * 2,
                               title_offset_y, title_offset_x);
        log_trace("Title creating status: %s.", ((title) ? ("SUCCESS") : ("FAIL")));
        box(title, 0, 0);

        // Inserting title text
        log_trace("LOGO printing started...");
        for (int cur_line = 0; cur_line < text_size_y; cur_line++) {
            mvwprintw(title, BORDER + cur_line, 1, "%s", title_text[cur_line]);
        }

        log_trace("LOGO printing ended.");
    }

    // Generating buttons
    int menu_size[2] = {MENU_SIZE_Y, MENU_SIZE_X};

    log_trace("Memory allocating for array of buttons started...");
    // It's okay to use sizeof in this case. We can say that this is a kind of automatic array data type detector!
    WINDOW **btns = calloc(btns_number, sizeof(btns[0]));
    log_trace("Memory allocating for array of buttons ended.");

    // Generating buttons array
    for (int i = 0; i < btns_number; i++) {
        prtybtn(btns_names[i], &btns[i], menu_container, menu_size, i, btns_number);
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

    log_trace("Memory cleaning for array of buttons started...");
    free(btns);
    log_trace("Memory cleaning for array of buttons ended.");

    log_info("%s function ended.", __PRETTY_FUNCTION__);

    return EOK;
}


enum BTNINFO {
    BTN_SIZE_Y,
    BTN_SIZE_X,
    BTN_OFFSET_Y,
    BTN_OFFSET_X
};

enum PRNTSIZE {
    PRNT_SIZE_Y,
    PRNT_SIZE_X
};

int prtybtn(const char *btn_name, WINDOW **btn, WINDOW *prnt,
            const int prnt_size[2], int btn_number, int max_btn_number) {

    log_trace("%s function started...", __PRETTY_FUNCTION__ );

    if (btn_number < 0 || btn_number > (max_btn_number - 1)) {
        log_fatal("Sequence number of current button is invalid."
                  " It is %d, maximum is %d.", btn_number, max_btn_number);

        return EBTNNMBR;
    }

    int btn_info[4] = {};


    // Generating button sizes
    btn_info[BTN_SIZE_Y] = 1 + BORDER * 2;

    // It's OK to use strlen, because we will not create a button
    // with text that goes beyond the size of the int type
    btn_info[BTN_SIZE_X] = (int)strlen(btn_name) + BORDER * 4;
    log_info("Size of the button being created is generated: %dx%d.",
             btn_info[BTN_SIZE_X], btn_info[BTN_SIZE_Y]);


    // Generating button offsets
    btn_info[BTN_OFFSET_Y] = prnt_size[PRNT_SIZE_Y] - (max_btn_number - btn_number) * (BORDER + btn_info[BTN_SIZE_Y]);
    btn_info[BTN_OFFSET_X] = (prnt_size[PRNT_SIZE_X] - btn_info[BTN_SIZE_X]) / 2;
    log_info("Offset of the button being created are generated: %dx%d.",
             btn_info[BTN_OFFSET_X], btn_info[BTN_OFFSET_Y]);


    // Generating button
    *btn = derwin(prnt, btn_info[BTN_SIZE_Y], btn_info[BTN_SIZE_X],
                  btn_info[BTN_OFFSET_Y], btn_info[BTN_OFFSET_X]);
    box(*btn, 0, 0);

    log_trace("%s function ended...",  __PRETTY_FUNCTION__ );

    return EOK;
}
