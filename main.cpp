#include <curses.h>
#include <vector>
#include <string>

struct catsprite {
    std::vector<std::string> sprite;

};

int main (){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(FALSE);

    catsprite cat = {
        { "=^. .^=",
          "=^- -^=", 
          "=^>.<^=",
          "=^o o^=",
        }
    };

    int currsprite = 0;

    int Xborder, Yborder;
    getmaxyx(stdscr, Yborder, Xborder);

    int Xpl = Xborder / 2;
    int Ypl = Yborder - 4;
    bool r = true;
    int blinktimer = 320;

    while (r){
        blinktimer--;
        if (blinktimer > 270){
            currsprite = 1;
        }
        else if (blinktimer == 0){
            blinktimer = 320;
        }
        else {
            currsprite = 0;
        }

        int ch = getch();
        
        if (ch == 'q'){
            r = 0;
        }
        if ((ch == 'a' || ch == KEY_LEFT) && Xpl > 3){
            Xpl--;
        }
        if ((ch == 'd' || ch == KEY_RIGHT) && Xpl < Xborder - 4){
            Xpl++;
        }

        erase();
        mvprintw(Ypl, Xpl - 3, "%s", cat.sprite[currsprite].c_str());
        wnoutrefresh(stdscr);
        doupdate();

        napms(16);
    }

    endwin();
    return 0;
}