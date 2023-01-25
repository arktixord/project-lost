#ifndef PROJECT_LOST_MENU_H
#define PROJECT_LOST_MENU_H

#include "core.h"

// see 'char TTL_txt[PATH_MAX][PATH_MAX] = {}' line comment in menu.c
enum CHARMAP_MATRIX
{
    CHARMAP_MATRIX_Y = 128,
    CHARMAP_MATRIX_X = 512,
};

// Menu bar generators. Return selected button number.
int prty_menu(int btns_number, const char *btns_names[]);
int tech_menu(int btns_number, const char *btns_names[]);

// Title generator. Return the error code specified in core.h
int prty_ttl(const char *ttl_name, WINDOW **ttl, WINDOW *prnt,
             const int prnt_size[2]);

// Button generator. Return the error code specified int core.h
int prty_btn(const char *btn_name, WINDOW **btn, WINDOW *prnt,
             const int prnt_size[2], int btn_number, int max_btn_number);

// Text from file detector. Return the error code specified int core.h
int txt_parse(FILE *fp, int txt_size[2], char txt[CHARMAP_MATRIX_Y][CHARMAP_MATRIX_X]);

#endif
