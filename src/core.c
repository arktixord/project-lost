#include "core.h"

static const char * LOG_FILE_NAME = "project_lost.log";

const char *ERR_MSG[] = {
        "Can't determine button number.",
        "No errors.",
        "Error with file working.",
        "Title/button name did not recognized.",
        "Terminal can't display colours.",
        "Error adding .log file.",
        "Too small terminal.",
        "Bad sequence number of the button being created.",
        "Unknown error."
};


int init() {
    // Working with LOGs
    loginit();

    // Checking terminal size
    CONSOLE_SCREEN_BUFFER_INFO console_scr_buf_info;
    int win_term_columns, win_term_rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_scr_buf_info);
    win_term_columns = console_scr_buf_info.srWindow.Right - console_scr_buf_info.srWindow.Left + 1;
    win_term_rows = console_scr_buf_info.srWindow.Bottom - console_scr_buf_info.srWindow.Top + 1;

    if (win_term_columns < MIN_TERM_X || win_term_rows < MIN_TERM_Y
            || win_term_columns * win_term_rows < MIN_TERM_AREA) {

        log_fatal("Treminal size is too small.");
        return ESMALLTERM;
    }
    log_info("Terminal size is OK");

    // Working with Curses
    log_trace("Curses init started...");
    initscr();
    noecho();
    cbreak();
    start_color();
    log_trace("Curses init ended.");

    return EOK;
}


void deinit() {
    nocbreak();
    echo();
    endwin();
}


int loginit() {
    FILE *fp = fopen(LOG_FILE_NAME, "w+");

    if (fp == NULL) {
        return EFILE;
    }

    fprintf(fp, "\n");
    if (log_add_fp(fp, LOG_TRACE)) {
        return ELOGFP;
    }

    log_set_quiet(TRUE);
    log_info("Logs are successfully initialized.");

    return EOK;
}


// TODO: Finish work with colours
//enum COLOUR {
//    PLAYER_CLR_PAIR = 1,
//    GRASS_CLR_PAIR,
//    STONE_CLR_PAIR,
//    WATER_CLR_PAIR
//};
//
//int colorinit() {
//    log_trace("%s function started...", __PRETTY_FUNCTION__ );
//
//    if (has_colors()) {
//        log_info("Terminal colours availible.");
//    } else {
//        log_warn("Terminal colours unavailible.");
//        return ECOLOR;
//    }
//
//    init_pair(PLAYER_CLR_PAIR, COLOR_);
//
//    log_trace("%s function ended.", __PRETTY_FUNCTION__ );
//}


const char * error_mailer(int err_num) {
    return ERR_MSG[((err_num > EUNKERR) ? (EUNKERR) : (err_num))];
}
