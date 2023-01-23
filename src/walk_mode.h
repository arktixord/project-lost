#ifndef PROJECT_LOST_WALK_MODE_H
#define PROJECT_LOST_WALK_MODE_H


#include "core.h"

int walk_window();

enum TERRAIN_TYPE {
    WATER,
    EARTH,
    PEAKS,
    TERR_N
};

enum TERRAIN_CHAR {
    CH_WATER = '~',
    CH_EARTH = ' ',
    CH_PEAKS = 'M',
    CH_PLAYER = '@'
};

typedef struct PLAYER {
    int x;
    int y;

    int alive;
    int god_mode;

    char place_holder;
} PLAYER;

typedef struct MAPSQUARE {
    int x;
    int y;
} MAPSQUARE;

int ** terrain_gen(int height, int width, int start_x, int start_y, int (*get_terrain_type) (int x, int y));
int terrain_display(WINDOW *walking_area, int window_size_y, int window_size_x, int window_border, int **map);
int get_terrain_type(int x, int y);

int remap(WINDOW *walk_area, PLAYER *boy, MAPSQUARE *cur_square,
          int walk_offset_y, int walk_offset_x, int direction);

int spawn_player(WINDOW *walk_area, PLAYER *boy, int offset_y, int offset_x);
int move_player(WINDOW *walk_area, PLAYER *boy, int offset_y, int offset_x, char direction);

#endif
