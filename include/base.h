#include <stdio.h>
#include <term.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

int start_screen(FILE *);
int print_center(const char*, int, const char*);
int basic_chat(WINDOW *);
int menu(WINDOW *);
int *make_server();