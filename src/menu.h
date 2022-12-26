#ifndef PROJECT_LOST_MENU_H
#define PROJECT_LOST_MENU_H

#include "core.h"

// Menu bar generators. Return selected button number.
int prty_menu(int btns_number, const char *btns_names[]);
int tech_menu(int btns_number, const char *btns_names[]);

// Title generator. Return OK or ERR values.
int prty_ttl(const char *ttl_name, WINDOW *ttl, WINDOW *prnt,
             const int prnt_size[2]);

// Button generator. Return OK or ERR values.
int prty_btn(const char *btn_name, WINDOW **btn, WINDOW *prnt,
             const int prnt_size[2], int btn_number, int max_btn_number);

// Text from file detector. Return OK or ERR values.
int txt_parse(FILE *fp, int txt_size[2], char txt[PATH_MAX][PATH_MAX]);

#endif
