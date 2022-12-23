#include "menu.h"
#include "core.h"
#include <stdio.h>

const char *SAVE_FILE_NAME = "../logos/SAVE.logo";
const char *QUIT_FILE_NAME = "../logos/QUIT.logo";
const char *TITLE_FILE_NAME = "../logos/LOST.logo";
const char *PAUSE_FILE_NAME = "../logos/PAUSE.logo";
const char *ERROR_FILE_NAME = "../logos/ERROR.logo";
const char *NEWGAME_FILE_NAME = "../logos/NEWGAME.logo";
const char *CONTINUE_FILE_NAME = "../logos/CONTINUE.logo";
const char *SETTINGS_FILE_NAME = "../logos/SETTINGS.logo";

enum MENUSIZE {
    MENU_SIZE_Y = MIN_TERM_Y,
    MENU_SIZE_X = MIN_TERM_X,
    BORDER = 1
};

enum TTLINFO {
    TTL_SIZE_Y,
    TTL_SIZE_X,
    TTL_OFFSET_Y,
    TTL_OFFSET_X
};

enum TXTSIZE {
    TXT_SIZE_Y,
    TXT_SIZE_X
};

int prtymenu(int btns_number, const char *btns_names[]) {
    log_info("%s function started...", __PRETTY_FUNCTION__ );

    /* Detecting terminal screen size */
    int term_y = LINES;
    int term_x = COLS;
    log_info("Terminal size detected: %dx%d.", term_x, term_y);

    /* GENERATING MENU SPACE STARTED */
    /* Generating menu offsets */
    // This way the menu will not be stretched over the entire terminal screen
    int menu_offset_y = (term_y - MENU_SIZE_Y) / 2;
    int menu_offset_x = (term_x - MENU_SIZE_X) / 2;
    log_info("Menu offsets generated: %dx%d.", menu_offset_x, menu_offset_y);

    /* Generating container for all menu elements */
    WINDOW *menu_container = derwin(stdscr, MENU_SIZE_Y, MENU_SIZE_X, menu_offset_y, menu_offset_x);
    log_trace("Container for menu creating status: %s.", ((menu_container) ? ("SUCSESS") : ("FAIL")));
    wbkgd(menu_container, ' ');
    box(menu_container, 0, 0);
    /* GENERATING MENU SPACE ENDED */


    /* GENERATING TITLE STARTED */
    /* Generating title size */
    int ttl_info[4] = {};
    int ttl_txt_size[2] = {};
    char ttl_txt[PATH_MAX][PATH_MAX] = {};

    FILE *fp = fopen(TITLE_FILE_NAME, "r");
    txtdtct(fp, ttl_txt_size, ttl_txt);
    int close_status = fclose(fp);
    log_trace("Title file closing status: %s.", ((close_status) ? ("FAIL") : ("SUCCESS")));

    /* Generating title offsets */
    // It should be at the top in the center of the menu
    ttl_info[TTL_OFFSET_Y] = 1;
    ttl_info[TTL_OFFSET_X] = (MENU_SIZE_X - ttl_txt_size[TXT_SIZE_X]) / 2;
    log_info("Title offsets generated: %dx%d.", ttl_info[TTL_OFFSET_X], ttl_info[TTL_OFFSET_Y]);

    /* Generating title sizes */
    ttl_info[TTL_SIZE_Y] = ttl_txt_size[TXT_SIZE_Y] + BORDER * 2;
    ttl_info[TTL_SIZE_X] = ttl_txt_size[TXT_SIZE_X] + BORDER * 2;

    /* Generating title window */
    WINDOW *title = derwin(menu_container,
                           ttl_info[TTL_SIZE_Y], ttl_info[TTL_SIZE_X],
                           ttl_info[TTL_OFFSET_Y], ttl_info[TTL_OFFSET_X]);
    log_trace("Title creating status: %s.", ((title) ? ("SUCCESS") : ("FAIL")));
    box(title, 0, 0);

    /* Inserting title text */
    log_trace("LOGO printing started...");
    for (int cur_line = 0; cur_line < ttl_txt_size[TXT_SIZE_Y]; cur_line++) {
        mvwprintw(title, BORDER + cur_line, 1, "%s", ttl_txt[cur_line]);
    }
    log_trace("LOGO printing ended.");
    /* GENERATING TITLE ENDED */


    /* GENERATING BUTTONS STARTED */
    int menu_size[2] = {MENU_SIZE_Y, MENU_SIZE_X};

    /* Generating buttons array */
    log_trace("Memory allocating for array of buttons started...");
    WINDOW **btns = calloc(btns_number, sizeof(btns[0])); // It's okay to use sizeof in this case.
                                                          // This is a kind of automatic array data type detector!
    log_trace("Memory allocating for array of buttons ended.");

    /* Printing buttons array */
    for (int i = 0; i < btns_number; i++) {
        prtybtn(btns_names[i], &btns[i], menu_container, menu_size, i, btns_number);
    }
    /* GENERATING BUTTONS ENDED */


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

    /* Validating button current number */
    if (btn_number < 0 || max_btn_number < 1 || btn_number > (max_btn_number - 1)) {
        log_fatal("Sequence number of current button is invalid."
                  " It is %d, maximum is %d.", btn_number, max_btn_number);

        return EBTNNMBR;
    }


    /* Generating button text */
    const char *cur_btn_name;
    int btn_info[4] = {};
    int btn_txt_size[2] = {};
    char btn_txt[PATH_MAX][PATH_MAX] = {};

    // Button name checking
    if (!strcmp(btn_name, "NEW GAME") || !strcmp(btn_name, "New game")) {
        cur_btn_name = NEWGAME_FILE_NAME;
    } else if (!strcmp(btn_name, "CONTINUE") || !strcmp(btn_name, "Continue")) {
        cur_btn_name = CONTINUE_FILE_NAME;
    } else if (!strcmp(btn_name, "SETTINGS") || !strcmp(btn_name, "Settings")) {
        cur_btn_name = SETTINGS_FILE_NAME;
    } else if (!strcmp(btn_name, "SAVE") || !strcmp(btn_name, "Save")) {
        cur_btn_name = SAVE_FILE_NAME;
    } else if (!strcmp(btn_name, "QUIT") || !strcmp(btn_name, "Quit")) {
        cur_btn_name = QUIT_FILE_NAME;
    } else {
        log_warn("Button name did not recognized.");
        cur_btn_name = ERROR_FILE_NAME;
    }

    // Button text detecting
    FILE *fp = fopen(cur_btn_name, "r");
    txtdtct(fp, btn_txt_size, btn_txt);
    int close_status = fclose(fp);
    log_trace("Button file closing status: %s.", ((close_status) ? ("FAIL") : ("SUCCESS")));

    // Generating button sizes
    btn_info[BTN_SIZE_Y] = btn_txt_size[TXT_SIZE_Y] + BORDER * 2;
    btn_info[BTN_SIZE_X] = btn_txt_size[TXT_SIZE_X] + BORDER * 2;
    log_info("Button size is generated: %dx%d.",
             btn_info[BTN_SIZE_X], btn_info[BTN_SIZE_Y]);

    // Generating button offsets
    btn_info[BTN_OFFSET_Y] = prnt_size[PRNT_SIZE_Y] - (max_btn_number - btn_number) * (BORDER + btn_info[BTN_SIZE_Y]);
    btn_info[BTN_OFFSET_X] = (prnt_size[PRNT_SIZE_X] - btn_info[BTN_SIZE_X]) / 2;
    log_info("Button offset generated: %dx%d.",
             btn_info[BTN_OFFSET_X], btn_info[BTN_OFFSET_Y]);

    // Generating button
    *btn = derwin(prnt, btn_info[BTN_SIZE_Y], btn_info[BTN_SIZE_X],
                  btn_info[BTN_OFFSET_Y], btn_info[BTN_OFFSET_X]);
    box(*btn, 0, 0);

    // Inserting text
    log_trace("Button text printing started...");
    for (int cur_line = 0; cur_line < btn_txt_size[BTN_SIZE_Y]; cur_line++) {
        int status = mvwprintw(*btn, btn_info[BTN_OFFSET_Y], btn_info[BTN_OFFSET_X], "%s", btn_txt[cur_line]);
        log_trace("Printing text in button status: %s.", ((status) ? ("FAIL") : ("SUCCESS")));
    }
    log_trace("Button text printing ended.");

    log_trace("%s function ended...",  __PRETTY_FUNCTION__ );

    return EOK;
}


void txtdtct(FILE *fp, int txt_size[2], char txt[PATH_MAX][PATH_MAX]) {
    log_trace("%s started...", __PRETTY_FUNCTION__ );

    txt_size[TXT_SIZE_Y] = 0;
    txt_size[TXT_SIZE_X] = 0;

    if (fp == NULL) {
        log_warn("Opening file failed. "
                 "Text size generated: 0x0");

        return;
    }

    int columns = 0;
    int lines = 0;
    int pos = 0;
    char ch;

    while ((ch = (char) fgetc(fp)) != EOF) {
        if (ch == '\n') {
            txt[lines++][pos] = '\0';
            columns = pos;
            pos = 0;
        } else {
            txt[lines][pos++] = (char) ch;
        }
    }

    if (pos) {
        txt[lines++][pos] = '\0';
        columns = 0;

        log_warn("No empty line at the end of file detected.");
    }

    txt_size[TXT_SIZE_Y] = lines;
    txt_size[TXT_SIZE_X] = columns;

    log_trace("%s ended.", __PRETTY_FUNCTION__ );
}
