#include "base.h"
#include "rssparse.h"

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

    WINDOW *big_win = newwin(win_size.y -4, win_size.x / 2, 0, 0);
    WINDOW *sm_win1 = newwin((win_size.y/2)-2 , win_size.x / 2, 0, (win_size.x/2));
    WINDOW *sm_win2 = newwin(win_size.y/2, win_size.x / 2, (win_size.y/2)-2, (win_size.x /2));
    WINDOW *cmd_bar = newwin(3, win_size.x, win_size.y - 3, 0);

    refresh();
    box(big_win, 0, 0);
    wrefresh(big_win);
    box(sm_win1, 0, 0);
    wrefresh(sm_win1);
    box(sm_win2, 0, 0);
    wrefresh(sm_win2);
    box(cmd_bar, 0, 0);
    wrefresh(cmd_bar);

    WINDOW *win_arr[] = {big_win, sm_win1, sm_win2, cmd_bar}; 

    weather(sm_win1);
    menu(big_win, win_arr);

	refresh();
	endwin();

    return 0;
}

int print_center(const char *word, int y_offset, const char *word2){
    mvprintw(win_size.y/2 - y_offset, win_size.x/2 - strlen(word)/2, word, word2);

    return 0;
}

int basic_chat(WINDOW * win){
    int y_size, x_size;
    getmaxyx( win, y_size, x_size );
    (void) x_size;

    //char messages[256][256];
    char message[256];

    while(strcmp(message, "\\quit")){ // == 0) || (strcmp(message, "/quit") == 0)){
        wclear(win);
        mvwprintw(win,1,1, "WELCOME TO ARISTOTLE---<\\quit> to exit\n");
        for (int i = 0; i <= n_message; i++){
            mvwprintw(win,i+3,1, "%s\n", msg_arr[i]);
        }
        mvwprintw(win, y_size - 2,1 ,"chat> ");
        box(win, 0, 0);
        wrefresh(win);
        wgetstr(win,message);
        if (strcmp(message, "\\quit") == 0) { return 0; }
        strcpy(msg_arr[n_message], message);
        n_message++;
    }
    return 0;

}

int menu(WINDOW * win, WINDOW ** win_arr){
    while(1){
        wclear(win);
        wprintw(win, "What would you like to do:\n");
        wprintw(win,"1) command prompt\n");
        wprintw(win,"2) chat\n");
        wprintw(win,"3) weather\n");
        wprintw(win,"4) national dept\n");
        wprintw(win,"5) inspirational messages\n");
        wprintw(win,"6) deep insights\n");
        wprintw(win,"q) quit\n");
        wprintw(win,"Select a number: ");
        wrefresh(win);
        char selection;
        selection = wgetch(win);

        switch(selection){
            case '1':
                cmd_prompt(win_arr[3], win_arr);
                break;
            case '2':
                basic_chat(win);
                break;
            case '3':
                weather(win);
                cmd_prompt(win_arr[3], win_arr);
                break;
            case '4':
                nationalDebt(win);
                cmd_prompt(win_arr[3], win_arr);
                break;
            case '5':
                ascii_print(win);
                cmd_prompt(win_arr[3], win_arr);
                break;
            case '6':
                rss_feed(win);
                cmd_prompt(win_arr[3], win_arr);
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

FILE* http_get(char* url, char* of){
        // Build the command
        char* wget = "wget -q -O ";
        char* space = " ";
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "%s%s%s%s", wget, of, space, url);
        //printf("%s", cmd);
        // Execute the command
        system(cmd);

        FILE* fp = fopen(of, "r");
        return fp;
}




int weather(WINDOW * win){
        char* url = "https://api.weather.gov/gridpoints/IWX/30,64/forecast";
        char* outfile = "weather.txt";
        FILE* weatherfp = http_get(url, outfile);
        char* forecast = "";
        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        int n = 0;
        int line_no = 2;

        //int y, x;
        //getbegyx(win,y,x);

        wclear(win);
        if (weatherfp == NULL) return(4);
        while ((read = getline(&line, &len, weatherfp)) != -1) {
            if(strstr(line, "detailedForecast")){
                 //printf("%s", line);



                forecast = strchr(line, ':');
                forecast = strchr(forecast, '"');
                forecast++;
                forecast = strtok(forecast, "\"");
                for(int i = 0; i <= strlen(forecast); i++){
                    if(forecast[i] == '.'){
                    forecast[i] = '\n';
                }
   }
        //fscanf(weatherfp, "detailedForecast", forecast);
                mvwprintw(win, line_no, 1, "%s\n", forecast);
                line_no++;
                n++;

            }
            if (strstr(line, "name")){
                forecast = strchr(line, ':');
                forecast = strchr(forecast, '"');
                forecast++;
                forecast = strtok(forecast, "\"");
                mvwprintw(win, line_no, 1, "%s\n", forecast);
                line_no++;
                n++;
            }

            if (n >= 6) break;
        }
        fclose(weatherfp);
        
        box(win, 0,0);
        wrefresh(win);
        return 0;
}

int nationalDebt(WINDOW * win){

        wclear(win);
        box(win, 0,0);

        char* url = "https://www.pgpf.org/national-debt-clock";
        char* outfile = "nationaldebt.html";
        FILE* debtfp = http_get(url, outfile);
        char* debt = "";
        char* line = NULL;
        size_t len = 0;
        ssize_t read;

        if (debtfp == NULL) return(4);
        while ((read = getline(&line, &len, debtfp)) != -1) {
            if(strstr(line, "ticker-text")){
                 //printf("%s", line);



                debt = strchr(line, '$');
                //forecast = strchr(forecast, '"');
                //forecast++;
                debt = strtok(debt, "<");
        //fscanf(weatherfp, "detailedForecast", forecast);
                mvwprintw(win, 2, 2, "The current national debt is:\n%s\n", debt);
                break;

            }

        }
        wrefresh(win);
        fclose(debtfp);

        return 0;
}

int cmd_prompt(WINDOW * win, WINDOW ** win_arr){

        //char cmd[256];

        wclear(win);
        box(win, 0,0);
        mvwprintw(win, 1, 1, "cmd> ");
        wrefresh(win);
        
        //wgetstr(win,cmd);
        char win_n = wgetch(win);

        //int i = 0;
        
        switch(win_n){
            case '1':
                menu(win_arr[0], win_arr);
                break;
            case '2':
                menu(win_arr[1], win_arr);
                break;
            case '3':
                menu(win_arr[2], win_arr);
                break;
            case 'q':
                return 0;
            default:
                cmd_prompt(win, win_arr);
        }

        return 0;
}

int ascii_print(WINDOW* win){
    int r = rand() % 3;

    wclear(win);
    char filename[64];
    sprintf(filename,"./ascii/%d.txt", r);
    FILE *fp = fopen(filename, "r");

    char buff[BUFSIZ];

    int line_n = 1;

    while(fgets(buff, BUFSIZ, fp)) {
        mvwprintw(win, line_n, 2, "%s", buff);
        line_n++;
    }

    fclose(fp);
    box(win, 0,0);
    wrefresh(win);
    return 0;
}

int rss_feed(WINDOW * win){

    wclear(win);

    const char *rss_feeds[] = {
        //"https://rdr453.github.io/rrizzo-blog/feed.xml",
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
