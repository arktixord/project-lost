#ifndef PROJECT_LOST_MENU_H
#define PROJECT_LOST_MENU_H

#include "core.h"

int prtybar(int btns_number, const char *btns_names[]);
int techbar(int btns_number, const char *btns_names[]);

// TODO: Make enum instead of comment below.
/*
 * prnt_size - array with parrent container sizes.
 * [0] - size of the container on the Y axis
 * [1] - size of the container on the X axis
 *
 * max_btn_number - maximum number of buttons allowed for the parrent container. I expect it to be 4.
 * btn_number - serial number of the button. Minimum - 0, Maximum - (max_btn_number - 1).
 */

int prtybtn(const char *btn_name, WINDOW **btn, WINDOW *prnt,
            const int prnt_size[], int btn_number, int max_btn_number);

#endif
