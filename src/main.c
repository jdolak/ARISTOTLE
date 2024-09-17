#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <term.h>

#include "base.h"
#include "server.h"

int main( int argc, char *argv[] ){

    if (argc > 1) {
        if (!strcmp("--local", argv[1])) {
            start_screen(1);
            return 0;
        }
    }
    printf("Starting the Aristotle Server...\n");
    make_server();
    return 0;
}
