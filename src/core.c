#include "core.h"

static const char * LOG_FILE_NAME = "project_lost.log";

const char *ERR_MSG[] = {
        "No errors detected",
        "Screen initialization error",
        "Error prohibiting the output of newly entered characters",
        "Error prohibiting character bufferisation",
        "Error in working with opened file",
        "The name of the button/title didn't recognized",
        "Terminal can't display colours",
        "Error trying to create a new window",
        "Error creating/editing .log file",
        "The terminal is too small to display the game interface",
        "Error in the sequence number of the button being created",
        "No ground in current game area detected",
        "Unknown error"
};

const char *ERR_VAL_MSG[] = {
        "No error values detected",
        "Сan't recognize the number of the selected button"
};


extern int init() {
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

        log_fatal("Too small terminal size.");
        return ESMALLTERM;
    }

    // Working with Curses screen and characters
    if (initscr() == NULL) {
        log_fatal("Screen initialization error.");
        return ESCR;
    }

    if (noecho() == ERR) {
        log_fatal("Disabling echo error.");
        return EECHO;
    }

    if (cbreak() == ERR) {
        log_fatal("Turning cbreak mode error.");
        return ECBREAK;
    }

    // Working with Curses colours in terminal
    if (colorinit() != EOK) {
        log_error("Colour initialization error.");
        return ECOLOR;
    }

    if (keypad(stdscr, TRUE) == ERR) {
        return EUNKERR;
    }

    return EOK;
}


void deinit() {
    nocbreak();
    echo();
    endwin();
}


extern int loginit() {
    FILE *fp = fopen(LOG_FILE_NAME, "w+");
    if (fp == NULL) {
        log_error("Log file creating/opening error.");
        return EFILE;
    }

    if (log_add_fp(fp, LOG_TRACE)) {
        log_error("Log stream to the file redirection error.");
        return ELOGFP;
    }

    log_set_quiet(TRUE);

    return EOK;
}


extern int colorinit() {
    if (has_colors() == FALSE) {
        log_warn("Terminal colours unavailible.");
        return ECOLOR;
    }

    if (start_color() == ERR) {
        log_error("Color initialization error.");
        return ECOLOR;
    }

    return EOK;
}


int get_norm_height(int x, int y, int norm_diameter, float (*perlin2d) (float x, float y, float freq, int depth)) {
    // 0.01 and 8 are the best numbers of frequency and depth for Perlin's Noise normalization.
    float perlin_res = (*perlin2d) ((float)x, (float)y, 0.09F, 2);

    return ((int)(perlin_res * norm_diameter) % norm_diameter);
}


const char * error_mailer(int err_num) {
    if (err_num < EOK) {
        return ERR_VAL_MSG[(err_num * (-1))];
    }

    return ERR_MSG[((err_num > EUNKERR) ? (EUNKERR) : (err_num))];
}
