#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <term.h>

#include "base.h"
#include "server.h"

int main( int argc, char *argv[] ){

    if (argc > 1) {
        if (!strcmp("--local", argv[1])) {
            start_screen(stdout);
            return 0;
        }
    }
    printf("Starting the Aristotle Server...\n");
    run_server();
    return 0;
}
