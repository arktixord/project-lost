#include "game.h"

extern int game_start() {
    int status;

    if ((status = init()) != EOK) {
        fprintf(stderr, "Init error: %s.\n", error_mailer(status));
        return status;
    }

    if (prty_menu(4, (const char *[]) {"NEW GAME", "CONTINUE", "SETTINGS", "QUIT"}) == ECURBTN) {
        fprintf(stderr, "Start menu work error.\n");
        return ECURBTN;
    }

    return EOK;
}


void game_end() {
    deinit();
}
