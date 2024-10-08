#include <stdio.h>
#include <libxml/parser.h>
#include <string.h>
#include "rssparse.h"

void parseRSS(const char *url, WINDOW * win, int y, int x) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    
    int line_n = 1;
    
    doc = xmlReadFile(url, NULL, 0);
    if (doc == NULL) {
        printf("Failed to parse the RSS feed from %s.\n", url);
        mvwprintw(win, line_n + y, x, "If u are reading this something failed,\n  there was supposed to be an RSS feed here.\n  Pls pretend...");
        line_n++;
        return;
    }

    root_element = xmlDocGetRootElement(doc);
    for (xmlNode *node = root_element->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && strcmp((char *)node->name, "item") == 0) {
            for (xmlNode *child = node->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((char *)child->name, "title") == 0) {
                        mvwprintw(win, line_n + y, x, "Title: %s\n", xmlNodeGetContent(child));
                        line_n++;
                    } else if (strcmp((char *)child->name, "link") == 0) {
                        mvwprintw(win, line_n + y, x, "Link: %s\n", xmlNodeGetContent(child));
                        line_n++;
                    } else if (strcmp((char *)child->name, "pubDate") == 0) {
                        mvwprintw(win, line_n + y, x, "Published: %s\n", xmlNodeGetContent(child));
                        line_n++;
                    }
                }
            }
        }
    }


    xmlFreeDoc(doc);
    xmlCleanupParser();
}

int rss_feed(WINDOW * win){

    wclear(win);

    const char *rss_feeds[] = {
        "https://rdr453.github.io/rrizzo-blog/feed.xml",
        "https://werdman25.wordpress.com/feed/",
        "",
        NULL // Marks end of the array
    };

    for (int i = 0; rss_feeds[i] != NULL; i++) {
        parseRSS(rss_feeds[i], win, 1, 1);
    }
    box(win, 0,0);
    wrefresh(win);
    return 0;

}

/*
int main() {
    const char *rss_feeds[] = {
        "https://rdr453.github.io/rrizzo-blog/feed.xml",
        "",
        NULL // Marks end of the array
    };

    for (int i = 0; rss_feeds[i] != NULL; i++) {
        printf("\nParsing feed: %s\n", rss_feeds[i]);
        parseRSS(rss_feeds[i]);
    }

    return 0;
}
*/


