#ifndef PROJECT_LOST_CORE_H
#define PROJECT_LOST_CORE_H

#include "../libs/pdcurses/curses.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "menu.h"
#include "log.h"

#if !defined(FALSE) && !defined(TRUE)
    enum BOOL {FALSE, TRUE};
#endif


enum MIN_TERM_SIZE {
    MIN_TERM_Y = 36,
    MIN_TERM_X = 64,
    MIN_TERM_AREA = MIN_TERM_Y * MIN_TERM_X
};

enum ERRORS {
    EOK,
    EFILE,
    ELOGFP,
    ESMALLTERM,
    EBTNNMBR,
    EUNKERR
};

int init();
void deinit();

int loginit();

const char * error_mailer(int err_num);

#endif
