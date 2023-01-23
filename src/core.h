#ifndef PROJECT_LOST_CORE_H
#define PROJECT_LOST_CORE_H

#if !defined(FALSE) && !defined(TRUE)
    enum BOOL {FALSE, TRUE};
#endif


#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../libs/pdcurses/curses.h"
#include "../libs/heightmap/height_map.h"
#include "walk_mode.h"
#include "menu.h"
#include "log.h"

enum MIN_TERM_SIZE {
    MIN_TERM_Y = 48,
    MIN_TERM_X = 86,
    MIN_TERM_AREA = MIN_TERM_Y * MIN_TERM_X
};

enum ERRORS {
    EOK,            // No errors
    ESCR,           // Error in screen init
    EECHO,          // Error in echo working
    ECBREAK,        // Error in cbreak working
    EFILE,          // Error in working with file
    ENAME,          // Error in title or button file name
    ECOLOR,         // Error in terminal colour working
    EWINCRT,        // Error in creating new curses window
    ELOGFP,         // Error in log file working
    ESMALLTERM,     // Error caused by small terminal size
    EBTNNMBR,       // Error caused by invalid number of button been created
    EGROUND,
    EUNKERR         // Unknown error
};

enum ERROR_VALUES {
    EVALOK,
    ECURBTN = -1,   // Error in menu working
};

extern int init();                      // Return the error code specified in core.h
void deinit();                          // Return the error code specified in core.h

extern int loginit();                   // Return the error code specified in core.h
extern int colorinit();                 // Return the error code specified in core.h

int get_norm_height(int x, int y,
                    int norm_diameter,
                    float (*perlin2d) (float x, float y, float freq, int depth));

const char * error_mailer(int err_num); // Return error message from ERR_MSG array in core.c


#endif
