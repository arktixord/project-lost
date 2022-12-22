#include "core.h"

static const char * LOG_FILE_NAME = "project_lost.log";

const char *ERR_MSG[] = {
        "No errors",
        "Error with file",
        "Error adding .log file",
        "Too small terminal",
        "Bad sequence number of the button being created",
        "Unknown error"
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
    log_info("Curses init started...");
    initscr();
    noecho();
    cbreak();
    log_info("Curses init ended.");

    return EOK;
}


void deinit() {
    nocbreak();
    echo();
    endwin();
}


int loginit() {
    FILE *fp = fopen(LOG_FILE_NAME, "a+");

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


const char * error_mailer(int err_num) {
    return ERR_MSG[((err_num > EUNKERR) ? (EUNKERR) : (err_num))];
}
