#include "game.h"

int game_start() {
    int status;

    if ((status = init()) != EOK) {
        fprintf(stderr, "Init error: %s.\n", error_mailer(status));
        return status;
    }

    int action;
    if ((action = prty_menu(2, (const char *[]) {"NEW GAME", "QUIT"})) == ECURBTN) {
        fprintf(stderr, "Start menu work error.\n");
        return ECURBTN;
    }
    log_info("Button %d selected.", action);

    // TODO: Implement error checking
    game_play(action);

    return EOK;
}

enum ACTIONS {
    START_GAME,
    QUIT_GAME
};

int game_play(int action) {
    if (action == QUIT_GAME) {
        log_trace("Quiting game.");
        game_end();
    } else if (action == START_GAME) {
        walk_window();
    }

    return EOK;
}

void game_end() {
    deinit();
}
