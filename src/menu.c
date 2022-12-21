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
    MENU_SIZE_Y = MIN_TERM_Y,
    MENU_SIZE_X = MIN_TERM_X,
    BORDER = 1
};

// TODO: Make text of the button size non constant. It will depend on img from file size.
enum BTNSIZE {
    TXT_SIZE_Y = 4,
    TXT_SIZE_X =
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

        log_trace("LINES COUNT: %d COLUMNS COUNT: %d POS COUNT: %d.", lines, columns, pos);
        if (pos) {
            title_text[lines++][pos] = '\0';
            columns = pos;

            log_trace("NO NEW LINE IN THE END DETECTED. LINES COUNT: %d COLUMNS COUNT: %d POS COUNT: %d.", lines, columns, pos);
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
    int btn_info[4] = {};
    int menu_size[2] = {MENU_SIZE_Y, MENU_SIZE_X};

    // TODO: Log memory work
    WINDOW **btns = calloc(btns_number, sizeof(btns[0]));

    for (int i = 0; i < btns_number; i++) {
        prtybtn(btns_names[i], &btns[i], menu_container, menu_size, i, btns_number);
    }

//    prtybtn(btns_names[0], btn_info, menu_size, 1, btns_number);
//    WINDOW *btn0 = derwin(menu_container, btn_info[2], btn_info[3], btn_info[0], btn_info[1]);
//    box(btn0, 0, 0);
//
//    prtybtn(btns_names[1], btn_info, menu_size, 2, btns_number);
//    WINDOW *btn1 = derwin(menu_container, btn_info[2], btn_info[3], btn_info[0], btn_info[1]);
//    box(btn1, 0, 0);
//
//    prtybtn(btns_names[2], btn_info, menu_size, 3, btns_number);
//    WINDOW *btn2 = derwin(menu_container, btn_info[2], btn_info[3], btn_info[0], btn_info[1]);
//    box(btn2, 0, 0);
//
//    prtybtn(btns_names[3], btn_info, menu_size, 4, btns_number);
//    WINDOW *btn3 = derwin(menu_container, btn_info[2], btn_info[3], btn_info[0], btn_info[1]);
//    box(btn3, 0, 0);

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

    // TODO: Log memory work
    free(btns);
    log_info("%s function ended.", __PRETTY_FUNCTION__);

    return EOK;
}

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
    btn_info[2] = 1 + BORDER * 2;

    // It's OK to use strlen, because we will not create a button
    // with text that goes beyond the size of the int type
    btn_info[3] = (int)strlen(btn_name) + BORDER * 4;
    log_info("Size of the button being created is generated: %dx%d.", btn_info[3], btn_info[2]);


    // Generating button offsets
    btn_info[0] = prnt_size[0] - (max_btn_number - btn_number) * (BORDER + btn_info[2]);
    btn_info[1] = (prnt_size[1] - btn_info[3]) / 2;
    log_info("Offset of the button being created are generated: %dx%d.", btn_info[1], btn_info[0]);


    *btn = derwin(prnt, btn_info[2], btn_info[3], btn_info[0], btn_info[1]);
    box(*btn, 0, 0);

    log_trace("%s function ended...",  __PRETTY_FUNCTION__ );

    return EOK;
}
