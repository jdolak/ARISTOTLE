#include "base.h"

int mode;
__thread Size win_size;

int start_screen(int fd){

    if (fd == 1) {
        newterm(NULL, stdout, stdin);
        getmaxyx( stdscr, win_size.y, win_size.x );
        mode = LOCAL;

    } else {
        FILE* fp = fdopen(fd, "r+");
        telnet_naws(fd, &win_size.y, &win_size.x);
        newterm(NULL, fp, fp);
        resizeterm(win_size.y, win_size.x);
        mode = SERVER;
    }

	print_center("Welcome to ARISTOTLE", 0, "");
	refresh();
    //sleep();
	print_center("Who are you?: ", -1, "");
	refresh();

    char name[64];
	getstr(name);

    clear();
    print_center("Welcome to ARISTOTLE %s", 0, name);

    WINDOW *big_win = newwin(win_size.y -1, win_size.x / 2, 0, 0);
    WINDOW *sm_win1 = newwin(win_size.y/2 , win_size.x / 2, 0, (win_size.x/2));
    WINDOW *sm_win2 = newwin(win_size.y/2, win_size.x / 2, (win_size.y/2), (win_size.x /2));

    refresh();
    box(big_win, 0, 0);
    wrefresh(big_win);
    box(sm_win1, 0, 0);
    wrefresh(sm_win1);
    box(sm_win2, 0, 0);
    wrefresh(sm_win2);
    menu(big_win);
	refresh();
	endwin();

    return 0;
}

int print_center(const char *word, int y_offset, const char *word2){
    mvprintw(win_size.y/2 - y_offset, win_size.x/2 - strlen(word)/2, word, word2);

    return 0;
}

int basic_chat(WINDOW * win){

    char messages[256][256];
    int n_message = 0;
    char message[256];

    while(strcmp(message, "\\quit")){ // == 0) || (strcmp(message, "/quit") == 0)){
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win,1,1, "WELCOME TO ARISTOTLE---<\\quit> to exit\n");
        for (int i = 0; i <= n_message; i++){
            mvwprintw(win,i+3,1, "%s\n", messages[i]);
        }
        mvwprintw(win,win_size.y - 2,1 ,"chat> ");
        wrefresh(win);
        wgetstr(win,message);
        strcpy(messages[n_message], message);
        n_message++;
    }
    return 0;

}

int menu(WINDOW * win){
    while(1){
        wclear(win);
        wprintw(win, "What would you like to do:\n");
        wprintw(win,"1) chat\n");
        wprintw(win,"q) quit\n");
        wprintw(win,"Select a number: ");
        wrefresh(win);
        char selection;
        selection = getch();

        switch(selection){
            case '1':
                basic_chat(win);
                break;
            case 'q':
                return 0;
        }
    }
    return 0;

}

int telnet_naws(int sockfd, int *length, int *width){

    uint8_t buffer[32];
    char will_naws[] = "\xFF\xFD\x1F";

    if (send(sockfd, will_naws, strlen(will_naws), 0) < 1) {
        printf("Error in sending NAWS.\n");
        printf("%s\n", strerror(errno));
        return 1;
    }
    if (recv(sockfd, buffer, sizeof(buffer) - 1, 0) < 1) {
        printf("Error in receiving NAWS.\n");
        printf("%s\n", strerror(errno));
        return 1;
    }
    *width = (buffer[6] << 8) | buffer[7];
    *length = (buffer[8] << 8) | buffer[9];

    return 0;
}

FILE* http_get(char* url, char* outfile){
    // Build the command
    char* wget = "wget -q -o ";
    char* wget2 = strstr(wget, outfile);
    char* wget3 = strstr(wget2, " ");
    char* cmd = strstr(wget3, url);

    // Execute the command
    system(cmd);
    
    FILE* fp = fopen("tmp", "r");
    return fp;
}
