#include <stdio.h>
#include <libxml/parser.h>
#include <string.h>
#include <ncurses.h>

void parseRSS(const char *url, WINDOW * win, int y, int x);
