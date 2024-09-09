#include <ncurses.h>
#include <stdio.h>
#include <strings.h>

#include "base.h"

int main( int argc, char *argv[] ){

    if (argc > 1) {
        if (!strcmp("--server", argv[1])) {
            printf("Server Mode.\n");
            return 0;
        }
    }

    start_screen();
    return 0;
}
