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

    getch();
	endwin();

    return 0;
}

int print_center(const char *word, int y_offset, const char *word2){
    int y_size, x_size;

    getmaxyx( stdscr, y_size, x_size );
    mvprintw(y_size/2 - y_offset, x_size/2 - strlen(word)/2, word, word2);

    return 0;
}

