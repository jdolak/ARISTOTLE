#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <term.h>
#include <stdlib.h>

#include "base.h"
#include "server.h"

char** make_chat_arr(){
    char **array = (char **)malloc(256 * sizeof(char *));
    if (!array) { return NULL; }

    for (int i = 0; i < 256; i++) {
        array[i] = (char *)calloc(256, sizeof(char));
        if (array[i] == NULL) {
            free(array);
            return NULL;
        }
    }
    return array;
}

int main( int argc, char *argv[] ){

    msg_arr = make_chat_arr(); 
    n_message = 0;

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