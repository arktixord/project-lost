#include "./src/core.h"

int main() {
    int result = init();
    if (result) {
        fprintf(stderr, "%s: %s: Init failed: %s\n", __FILE__, __FUNCTION__, error_mailer(result));
    }

    prtymenu(4, (const char *[]) {"NEW GAME", "CONTINUE", "SETTINGS", "QUIT"});
    getchar();

    deinit();

    return 0;
}
