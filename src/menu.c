#include "menu.h"

static const char *STR_SAVE = "SAVE";
static const char *STR_QUIT = "QUIT";
static const char *STR_LOST = "LOST";
static const char *STR_PAUSE = "PAUSE";
static const char *STR_ERROR = "ERROR";
static const char *STR_NEWGAME = "NEW GAME";
static const char *STR_CONTINUE = "CONTINUE";
static const char *STR_SETTINGS = "SETTINGS";

static const char *SAVE_FILE_NAME = "../logos/SAVE.logo";
static const char *QUIT_FILE_NAME = "../logos/QUIT.logo";
static const char *BRAND_FILE_NAME = "../logos/LOST.logo";
static const char *PAUSE_FILE_NAME = "../logos/PAUSE.logo";
static const char *ERROR_FILE_NAME = "../logos/ERROR.logo";
static const char *NEWGAME_FILE_NAME = "../logos/NEWGAME.logo";
static const char *CONTINUE_FILE_NAME = "../logos/CONTINUE.logo";
static const char *SETTINGS_FILE_NAME = "../logos/SETTINGS.logo";


enum MENUSIZE {
    MENU_SIZE_Y = MIN_TERM_Y,
    MENU_SIZE_X = MIN_TERM_X,
    BORDER = 1
};

enum TXTSIZE {
    TXT_SIZE_Y,
    TXT_SIZE_X
};

// Alas, when reading ENTER from me, the KEY_ENTER code is not perceived as this key.
// The solution I found says that instead of the KEY_ENTER code written in <curses.h>, you need to use code 10.
#define ENTER 10

int prty_menu(int btns_number, const char *btns_names[]) {
    // Detecting terminal screen size
    int term_y = LINES;
    int term_x = COLS;
    log_info("Terminal size detected: %dx%d.", term_x, term_y);

    // Generating menu offsets
    int menu_offset_y = (term_y - MENU_SIZE_Y) / 2; // This way the menu will not be
    int menu_offset_x = (term_x - MENU_SIZE_X) / 2; // stretched over the entire terminal screen
    log_info("Menu offsets generated: %dx%d.", menu_offset_x, menu_offset_y);

    // Declaring buttons beforehand, so that goto doesn't use them unitialized
    // Cricial to use before first 'goto error'
    WINDOW **btns = NULL;
    // success -- likewise
    // Variable responsible for definition of ERROR or OK code.
    int success = 0;

    // Generating container for all menu elements
    WINDOW *menu_container = derwin(stdscr, MENU_SIZE_Y, MENU_SIZE_X, menu_offset_y, menu_offset_x);
    if (menu_container == NULL) {
        log_error("Menu container creating error.");
        goto error;
    }

    wbkgd(menu_container, ' ');
    box(menu_container, 0, 0);

    // Generating title
    int menu_size[2] = {MENU_SIZE_Y, MENU_SIZE_X};
    WINDOW *title = NULL;
    prty_ttl(STR_LOST, &title, menu_container, menu_size);

    // Generating buttons array
    btns = calloc(btns_number, sizeof(btns[0])); // It's okay to use sizeof in this case.
                                                          // This is a kind of automatic array data type detector!
    if (btns == NULL) {
        log_error("Buttons array memory allocation.");
        goto error;
    }

    // Printing buttons array
    for (int i = 0; i < btns_number; i++) {
        prty_btn(btns_names[i], &btns[i], menu_container, menu_size, i, btns_number);
    }

    curs_set(FALSE); // hide cursor while using menu

    // Menu template ready
    if (refresh() == ERR) {
        log_error("Menu window refreshing error.");
        goto error;
    }

    // Selecting button by user
    int cur_btn = 0;
    if (box(btns[cur_btn], 0, 0) == ERR) {
        log_error("First button box drawing error.");
        goto error;
    }

    if (wrefresh(btns[cur_btn]) == ERR) {
        log_error("First button box refreshing error.");
        goto error;
    }

    keypad(menu_container, TRUE);
    int ch = 0;
    while (1) {
        ch = wgetch(menu_container);

        // It's OK not to use default in this switch construct,
        // since we're waiting for the user to select the button by pressing ENTER.
        switch (ch) {
            case KEY_UP:
                log_trace("Key UP pressed.");

                // "Removing" box of the current button
                if (wborder(btns[cur_btn], ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ') == ERR) {
                    log_error("Current button box clearing error");
                    goto error;
                }

                if (wrefresh(btns[cur_btn]) == ERR) {
                    log_error("Current button box clear refreshing error");
                    goto error;
                }

                if (--cur_btn < 0) {
                    cur_btn += btns_number;
                }

                // "Drawing" box of the new button
                if (box(btns[cur_btn], 0, 0) == ERR) {
                    log_error("New button box drawing error.");
                    goto error;
                }

                if (wrefresh(btns[cur_btn]) == ERR) {
                    log_error("New button box refreshing error.");
                    goto error;
                }

                break;

            case KEY_DOWN:
                log_trace("Key DOWN pressed.");

                // "Removing" box of the current button
                if (wborder(btns[cur_btn], ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ') == ERR) {
                    log_error("Current button box clearing error");
                    goto error;
                }

                if (wrefresh(btns[cur_btn]) == ERR) {
                    log_error("Current button box clear refreshing error");
                    goto error;
                }

                if (++cur_btn >= btns_number) {
                    cur_btn -= btns_number;
                }

                // "Drawing" box of the new button
                if (box(btns[cur_btn], 0, 0) == ERR) {
                    log_error("New button box drawing error.");
                    goto error;
                }

                if (wrefresh(btns[cur_btn]) == ERR) {
                    log_error("New button box refreshing error.");
                    goto error;
                }

                break;

            case ENTER:
                log_trace("Key ENTER pressed.");
                success = 1;

                goto end;
        }
    }

    error:
    end:

    curs_set(TRUE); // show again on exit

    wclear(title);
    delwin(title);

    if (btns != NULL) {
        for (int i = 0; i < btns_number; i++) {
            if (btns[i] != NULL) {
                wclear(btns[i]);
                delwin(btns[i]);
            }
        }

        free(btns);
    }

    wclear(menu_container);
    delwin(menu_container);

    if (success) {
        return cur_btn;
    }

    return ECURBTN;
}


enum PRNTSIZE {
    PRNT_SIZE_Y,
    PRNT_SIZE_X
};

enum TTLINFO {
    TTL_SIZE_Y,
    TTL_SIZE_X,
    TTL_OFFSET_Y,
    TTL_OFFSET_X
};

// TODO: Replace log_info with a full-fledged check with an if-construct
int prty_ttl(const char *ttl_name, WINDOW **ttl, WINDOW *prnt, const int prnt_size[2]) {
    const char *cur_ttl_name = NULL;

    // Title name checking
    if (!strcmp(ttl_name, STR_LOST)) {
        cur_ttl_name = BRAND_FILE_NAME;
    } else if (!strcmp(ttl_name, "PAUSE")) {
        cur_ttl_name = PAUSE_FILE_NAME;
    } else {
        log_error("Title name was not recognized.");
        return ENAME;
    }

    int ttl_info[4] = {};
    int ttl_txt_size[2] = {};

/*
 *  char TTL_txt[PATH_MAX][PATH_MAX] = {};
 *
 *  This piece of art caused SEGFAULT in stackframe because on some machines
 *  PATH_MAX might be huge [4096] and thus this matrix would take 4096*4096 = 
 *  = 16777216 bytes = 16384 KB = 16 MB, which seems little, but is a lot AF
 *
 *  Recommended maxium stack occupation per use is 64 KB. Therefore, chose to
 *  switch to 128*512 matrix, which is enough and occupies just fine
 */

    char TTL_txt[CHARMAP_MATRIX_Y][CHARMAP_MATRIX_X] = {};

    // Button text detecting
    FILE *fp = fopen(cur_ttl_name, "r");
    if (fp == NULL) {
        log_error("Can't open file [%s] for reading to get title", cur_ttl_name);
    } else {
        txt_parse(fp, ttl_txt_size, TTL_txt);
        if (fclose(fp) == ERR) {
            log_error("Title file closing error.");
            return EFILE;
        }
    }

    // Generating button sizes
    ttl_info[TTL_SIZE_Y] = ttl_txt_size[TXT_SIZE_Y] + BORDER * 2;
    ttl_info[TTL_SIZE_X] = ttl_txt_size[TXT_SIZE_X] + BORDER * 2;
    log_info("Title size is generated: %dx%d.",
             ttl_info[TTL_SIZE_X], ttl_info[TTL_SIZE_Y]);

    // Generating button offsets
    ttl_info[TTL_OFFSET_Y] = 1;
    ttl_info[TTL_OFFSET_X] = (prnt_size[PRNT_SIZE_X] - ttl_info[TTL_SIZE_X]) / 2;
    log_info("Title offset generated: %dx%d.",
             ttl_info[TTL_OFFSET_X], ttl_info[TTL_OFFSET_Y]);

    // Generating button
    *ttl = derwin(prnt, ttl_info[TTL_SIZE_Y], ttl_info[TTL_SIZE_X],
                  ttl_info[TTL_OFFSET_Y], ttl_info[TTL_OFFSET_X]);

    if (*ttl == NULL) {
        log_error("Title window creating error.");
        return EWINCRT;
    }

    // Inserting text
    for (int cur_line = 0; cur_line < ttl_txt_size[TTL_SIZE_Y]; cur_line++) {
        if (mvwprintw(*ttl, BORDER + cur_line, BORDER, "%s\n", TTL_txt[cur_line]) == ERR) {
            log_warn("Title text current line printing error.");
        }
    }

    if (box(*ttl, 0, 0) == ERR) {
        log_error("Title box painting error.");
    }
    
    return EOK;
}


enum BTNINFO {
    BTN_SIZE_Y,
    BTN_SIZE_X,
    BTN_OFFSET_Y,
    BTN_OFFSET_X
};

int prty_btn(const char *btn_name, WINDOW **btn, WINDOW *prnt,
             const int prnt_size[2], int btn_number, int max_btn_number) {

    // Validating button current number
    if (btn_number < 0 || max_btn_number < 1 || btn_number > (max_btn_number - 1)) {
        log_fatal("Number of current button is invalid."
                  " It is %d, maximum is %d.", btn_number, max_btn_number);

        return EBTNNMBR;
    }

    // Generating button text
    const char *cur_btn_name;

    // Button name checking
    if (!strcmp(btn_name, STR_NEWGAME)) {
        cur_btn_name = NEWGAME_FILE_NAME;
    } else if (!strcmp(btn_name, STR_CONTINUE)) {
        cur_btn_name = CONTINUE_FILE_NAME;
    } else if (!strcmp(btn_name, STR_SETTINGS)) {
        cur_btn_name = SETTINGS_FILE_NAME;
    } else if (!strcmp(btn_name, STR_SAVE)) {
        cur_btn_name = SAVE_FILE_NAME;
    } else if (!strcmp(btn_name, STR_QUIT)) {
        cur_btn_name = QUIT_FILE_NAME;
    } else {
        log_error("Button %d name was not recognized.", btn_number);
        return ENAME;
    }

    int btn_info[4] = {};
    int btn_txt_size[2] = {};
    char btn_txt[CHARMAP_MATRIX_Y][CHARMAP_MATRIX_X] = {};

    // Button text detecting
    FILE *fp = fopen(cur_btn_name, "r");
    txt_parse(fp, btn_txt_size, btn_txt);
    if (fclose(fp) == ERR) {
        log_error("Button %d text file closing error.", btn_number);
    }

    // Generating button sizes
    btn_info[BTN_SIZE_Y] = btn_txt_size[TXT_SIZE_Y] + BORDER * 2;
    btn_info[BTN_SIZE_X] = btn_txt_size[TXT_SIZE_X] + BORDER * 2;
    log_info("Button %d size generated: %dx%d.", btn_number, btn_info[BTN_SIZE_X], btn_info[BTN_SIZE_Y]);

    // Generating button offsets
    btn_info[BTN_OFFSET_Y] = prnt_size[PRNT_SIZE_Y] -
                             (max_btn_number - btn_number) * (BORDER + btn_info[BTN_SIZE_Y]);
    btn_info[BTN_OFFSET_X] = (prnt_size[PRNT_SIZE_X] - btn_info[BTN_SIZE_X]) / 2;
    log_info("Button %d offset generated: %dx%d.", btn_number, btn_info[BTN_OFFSET_X], btn_info[BTN_OFFSET_Y]);

    // Generating button
    *btn = derwin(prnt, btn_info[BTN_SIZE_Y], btn_info[BTN_SIZE_X],
                  btn_info[BTN_OFFSET_Y], btn_info[BTN_OFFSET_X]);

    if (btn == NULL) {
        log_error("Button %d window creating error.", btn_number);
        return EWINCRT;
    }

    // Inserting text
    for (int cur_line = 0; cur_line < btn_txt_size[BTN_SIZE_Y]; cur_line++) {
        if (mvwprintw(*btn, BORDER + cur_line, BORDER, "%s\n", btn_txt[cur_line]) == ERR) {
            log_warn("Button %d text current line printing error.", btn_number);
        }
    }

    return EOK;
}


int txt_parse(FILE *fp, int txt_size[2], char txt[CHARMAP_MATRIX_Y][CHARMAP_MATRIX_X]) {
    txt_size[TXT_SIZE_Y] = 0;
    txt_size[TXT_SIZE_X] = 0;

    if (fp == NULL) {
        log_error("Opening file error.");
        return EFILE;
    }

    int columns = 0;
    int lines = 0;
    int pos = 0;
    char ch;

    // TODO: Rework line length generation
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

    return EOK;
}
