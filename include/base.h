#include <stdio.h>
#include <term.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

typedef struct Size {
    int y;
    int x;
} Size;

enum {
    LOCAL,
    SERVER
};

char** msg_arr;
int n_message;

//int start_screen(FILE *);
int start_screen(int);
int print_center(const char*, int, const char*);
int basic_chat(WINDOW *);
int menu(WINDOW *, WINDOW **);
int make_server(void);
int telnet_naws(int, int *, int *);
FILE* http_get(char*, char*);
int weather(WINDOW *);
int nationalDebt(WINDOW *);
int cmd_prompt(WINDOW *, WINDOW **);
int ascii_print(WINDOW* win);
int rss_feed(WINDOW * );
