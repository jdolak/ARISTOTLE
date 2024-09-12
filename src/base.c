#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "base.h"

int start_screen(FILE *fd){

    if (fd == stdout) {
        newterm(NULL, stdout, stdin);
    } else {
        newterm(NULL, fd, fd);
    }
    //setterm(scr);
	print_center("Welcome to ARISTOTLE", 0, "");
	refresh();
    sleep(1);
	print_center("Who are you?: ", -1, "");
	refresh();

    char name[64];
	getstr(name);

    clear();
    print_center("Welcome to ARISTOTLE %s", 0, name);
    sleep(1);

    basic_chat(fd);
	refresh();
	endwin();

    return 0;
}

int print_center(const char *word, int y_offset, const char *word2){
    int y_size, x_size;

    getmaxyx( stdscr, y_size, x_size );
    mvprintw(y_size/2 - y_offset, x_size/2 - strlen(word)/2, word, word2);

    return 0;
}

int basic_chat(FILE *fd){
    //int y_size, x_size;
    //getmaxyx( stdscr, y_size, x_size );

    char messages[256][256];
    int n_message = 0;
    char message[256];

    clear();
    while(strcmp(message, "\\quit")){
        clear();
        printw("WELCOME TO ARISTOTLE---<\\quit> to exit\n");
        for (int i = 0; i <= n_message; i++){
            printw("%s\n", messages[i]);
        }
        getstr(message);
        strcpy(messages[n_message], message);
        n_message++;
        refresh();
    }
    return 0;

}

