#include "walk_mode.h"

enum WALKSIZE {
    WALK_SIZE_Y = MIN_TERM_Y,
    WALK_SIZE_X = MIN_TERM_X,
    BORDER = 1
};

int walk_window() {
    // Detecting terminal screen size
    int term_y = LINES;
    int term_x = COLS;
    log_info("Walk size X is %d, Walk size Y is %d.", WALK_SIZE_X, WALK_SIZE_Y);

    // Generating window offsets
    int walk_offset_y = (term_y - WALK_SIZE_Y) / 2; // This way the walk window will not be
    int walk_offset_x = (term_x - WALK_SIZE_X) / 2; // stretched over the entire terminal screen
    log_info("Walk offsets generated: %dx%d.", walk_offset_x, walk_offset_y);

    // Generating container for all window elements
    WINDOW *walk_window = derwin(stdscr, WALK_SIZE_Y, WALK_SIZE_X, walk_offset_y, walk_offset_x);
    if (walk_window == NULL) {
        log_error("Walk window creating error.");
        goto error;
    }

    wbkgd(walk_window, ' ');
    box(walk_window, 0, 0);

    // TODO: To think about possibility of colour display
    int **map = terrain_gen(WALK_SIZE_Y - BORDER * 2,
                            WALK_SIZE_X - BORDER * 2,
                            0, 0, get_terrain_type);

    terrain_display(walk_window, WALK_SIZE_Y,
                    WALK_SIZE_X, BORDER, map);

    MAPSQUARE current_square = {.x = 0, .y = 0};

    if (refresh() == ERR) {
        log_error("Walk window refreshing error.");
        goto error;
    }

    PLAYER boy = {};
    // TODO: Check for ground
    log_info("Spawn player status is %d", spawn_player(walk_window, &boy, BORDER, BORDER));

    while(1) {
        int detected_symbol = wgetch(walk_window);
        char direction = 's';

        switch (detected_symbol) {
            case KEY_UP:
                direction = 'u';
                break;

            case KEY_DOWN:
                direction = 'd';
                break;

            case KEY_RIGHT:
                direction = 'r';
                break;

            case KEY_LEFT:
                direction = 'l';
                break;

            case 'q':
                goto end;
                break;

            default:
                break;
        }

        int move_result;
        if ((move_result = move_player(walk_window, &boy, BORDER, BORDER, direction)) != EOK) {
            remap(walk_window, &boy, &current_square,
                  BORDER, BORDER, move_result);
        }
    }

    error:
    end:

    wclear(walk_window);
    delwin(walk_window);

    // TODO: Error handling
    return EOK;
}


int terrain_display(WINDOW *walking_area, int window_size_y, int window_size_x, int window_border, int **map) {
    for (int cur_line = window_size_y - window_border * 2 - 1; cur_line >= 0; cur_line--) {
        for (int cur_symbol = 0; cur_symbol < window_size_x - window_border * 2; cur_symbol++) {
            switch (map[cur_line][cur_symbol]) {
                case PEAKS:
                    mvwaddch(walking_area, cur_line + window_border, cur_symbol + window_border, CH_PEAKS);
                    break;

                case WATER:
                    mvwaddch(walking_area, cur_line + window_border, cur_symbol + window_border, CH_WATER);
                    break;

                default:
                    mvwaddch(walking_area, cur_line + window_border, cur_symbol + window_border, CH_EARTH);
                    break;
            }
        }

        free(map[cur_line]);
    }

    free(map);
}


int ** terrain_gen(int height, int width, int start_x, int start_y, int (*get_terrain_type) (int x, int y)) {
    // TODO: malloc func check
    int **terrain = malloc(height * sizeof(terrain[0]));

    for (int i = 0; i < height; i++) {
        // TODO: malloc func check
        terrain[i] = malloc(width * sizeof(terrain[i][0]));

        for (int k = 0; k < width; k++) {
            terrain[i][k] = get_terrain_type(k + start_x, i + start_y);
        }
    }

    return terrain;
}


// TODO: Config file with important stuff
int get_terrain_type(int x, int y) {
    // 300 is heuristically chosen for optimal dispersion.
    int res = get_norm_height(x, y, 300, perlin2d);

    // Less accent -> more material
    int peak_accent = 10;

    // Less accent -> less material
    int water_accent = -10;

    if (abs(res) > (300 - (300 / 3) + peak_accent)) {
        return PEAKS;
    } else if (abs(res) < (300 / 3) + water_accent) {
        return WATER;
    } else {
        return EARTH;
    }
}


int remap(WINDOW *walk_area, PLAYER *boy, MAPSQUARE *cur_square,
          int walk_offset_y, int walk_offset_x, int direction) {

    int height, width;
    getmaxyx(walk_area, height, width);

    switch (direction) {
        case 'u': {
            cur_square->y++;
            int **map = terrain_gen(height, width, width * cur_square->x,
                                    height * cur_square->y, get_terrain_type);

            terrain_display(walk_area, height, width, BORDER, map);
            boy->y = height - 2 * walk_offset_y;

            char symbol = mvwinch(walk_area, boy->y, boy->x) & A_CHARTEXT;
            boy->place_holder = symbol;

            mvwaddch(walk_area, boy->y, boy->x, CH_PLAYER);
            wrefresh(walk_area);

            break;
        }

        case 'd': {
            cur_square->y--;
            int **map = terrain_gen(height, width, width * cur_square->x,
                                    height * cur_square->y, get_terrain_type);

            terrain_display(walk_area, height, width, BORDER, map);
            boy->y = walk_offset_y;

            char symbol = mvwinch(walk_area, boy->y, boy->x) & A_CHARTEXT;
            boy->place_holder = symbol;

            mvwaddch(walk_area, boy->y, boy->x, CH_PLAYER);
            wrefresh(walk_area);

            break;
        }

        case 'l': {
            cur_square->x--;
            int **map = terrain_gen(height, width, width * cur_square->x,
                                    height * cur_square->y, get_terrain_type);

            terrain_display(walk_area, height, width, BORDER, map);
            boy->x = width - 2 * walk_offset_x;

            char symbol = mvwinch(walk_area, boy->y, boy->x) & A_CHARTEXT;
            boy->place_holder = symbol;

            mvwaddch(walk_area, boy->y, boy->x, CH_PLAYER);
            wrefresh(walk_area);

            break;
        }

        case 'r': {
            cur_square->x++;
            int **map = terrain_gen(height, width, width * cur_square->x,
                                    height * cur_square->y, get_terrain_type);

            terrain_display(walk_area, height, width, BORDER, map);
            boy->x = walk_offset_x;

            char symbol = mvwinch(walk_area, boy->y, boy->x) & A_CHARTEXT;
            boy->place_holder = symbol;

            mvwaddch(walk_area, boy->y, boy->x, CH_PLAYER);
            wrefresh(walk_area);

            break;
        }
    }
}


int spawn_player(WINDOW *walk_area, PLAYER *boy, int offset_y, int offset_x) {
    int height, width;
    getmaxyx(walk_area, height, width);
    log_info("Spawn got window with height %d and width %d", height, width);

    int k, i, holy_fucking_ground = 0;
    for (k = 0 + offset_x; k < width - offset_x; k++) {
        for (i = 0 + offset_y; i < height - offset_y; i++) {
            chtype read_symbol = mvwinch(walk_area, i, k);

            log_trace("[%d][%d]", i, k);
            char symbol = read_symbol & A_CHARTEXT;
            log_trace("Selected symbol is %c", symbol);
            if (symbol == CH_EARTH) {
                goto end;
            }
        }
    }

    return EGROUND;

    end:
    boy->x = k;
    boy->y = i;

    boy->god_mode = 0;
    boy->alive = 1;

    boy->place_holder = ' ';

    mvwaddch(walk_area, boy->y, boy->x, CH_PLAYER);
    wrefresh(walk_area);

    return EOK;
}


int move_player(WINDOW *walk_area, PLAYER *boy, int offset_y, int offset_x, char direction) {
    switch (direction) {
        case 'u': {
            if ((boy->y - 1) < offset_y) {
                return 'u';
            }

            char new_symbol = mvwinch(walk_area, boy->y - 1, boy->x) & A_CHARTEXT;
            if (new_symbol == CH_EARTH || boy->god_mode) {
                mvwaddch(walk_area, boy->y, boy->x, boy->place_holder);
                boy->place_holder = new_symbol;
                mvwaddch(walk_area, --boy->y, boy->x, CH_PLAYER);
            }

            break;
        }

        case 'd': {
            int height, width;
            getmaxyx(walk_area, height, width);

            if ((boy->y + 1) >= height - offset_y) {
                return 'd';
            }

            char new_symbol = mvwinch(walk_area, boy->y + 1, boy->x) & A_CHARTEXT;
            if (new_symbol == CH_EARTH || boy->god_mode) {
                mvwaddch(walk_area, boy->y, boy->x, boy->place_holder);
                boy->place_holder = new_symbol;
                mvwaddch(walk_area, ++boy->y, boy->x, CH_PLAYER);
            }

            break;
        }

        case 'l': {
            int height, width;
            getmaxyx(walk_area, height, width);

            if ((boy->x - 1) < offset_x) {
                return 'l';
            }

            char new_symbol = mvwinch(walk_area, boy->y, boy->x - 1) & A_CHARTEXT;
            if (new_symbol == CH_EARTH || boy->god_mode) {
                mvwaddch(walk_area, boy->y, boy->x, boy->place_holder);
                boy->place_holder = new_symbol;
                mvwaddch(walk_area, boy->y, --boy->x, CH_PLAYER);
            }

            break;
        }

        case 'r': {
            int height, width;
            getmaxyx(walk_area, height, width);

            if ((boy->x + 1) >= width - offset_x) {
                return 'r';
            }

            char new_symbol = mvwinch(walk_area, boy->y, boy->x + 1) & A_CHARTEXT;
            if (new_symbol == CH_EARTH || boy->god_mode) {
                mvwaddch(walk_area, boy->y, boy->x, boy->place_holder);
                boy->place_holder = new_symbol;
                mvwaddch(walk_area, boy->y, ++boy->x, CH_PLAYER);
            }

            break;
        }
    }

    wrefresh(walk_area);

    return EOK;
}