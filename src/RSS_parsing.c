#include <stdio.h>
#include <libxml/parser.h>

void parseRSS(const char *url) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    
    doc = xmlReadFile(url, NULL, 0);
    if (doc == NULL) {
        printf("Failed to parse the RSS feed from %s.\n", url);
        return;
    }

    root_element = xmlDocGetRootElement(doc);
    for (xmlNode *node = root_element->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && strcmp((char *)node->name, "item") == 0) {
            for (xmlNode *child = node->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((char *)child->name, "title") == 0) {
                        printf("Title: %s\n", xmlNodeGetContent(child));
                    } else if (strcmp((char *)child->name, "link") == 0) {
                        printf("Link: %s\n", xmlNodeGetContent(child));
                    } else if (strcmp((char *)child->name, "pubDate") == 0) {
                        printf("Published: %s\n", xmlNodeGetContent(child));
                    }
                }
            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
}

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
