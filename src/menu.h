#ifndef PROJECT_LOST_MENU_H
#define PROJECT_LOST_MENU_H

#include "core.h"

// Menu bar generators
int prtymenu(int btns_number, const char *btns_names[]);
int techmenu(int btns_number, const char *btns_names[]);

// Button generator
int prtybtn(const char *btn_name, WINDOW **btn, WINDOW *prnt,
            const int prnt_size[2], int btn_number, int max_btn_number);

// Text from file detector
void txtdtct(FILE *fp, int txt_size[2], char txt[PATH_MAX][PATH_MAX]);

#endif
