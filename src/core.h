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
    MIN_TERM_Y = 48,
    MIN_TERM_X = 86,
    MIN_TERM_AREA = MIN_TERM_Y * MIN_TERM_X
};

enum ERRORS {
    ECURBTN = -1,
    EOK = 0,
    EFILE,
    ENAME,
    ECOLOR,
    ELOGFP,
    ESMALLTERM,
    EBTNNMBR,
    EUNKERR
};

int init();
void deinit();

int loginit();
//int colorinit();

const char * error_mailer(int err_num);

#endif
