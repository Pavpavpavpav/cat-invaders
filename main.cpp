#include <ncurses.h>

int main (){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(FALSE);

    int Xborder, Yborder;
    getmaxyx(stdscr, Xborder, Yborder);

    int Xpl = Xborder / 2;
    int Ypl = Yborder - 2;
    bool r = true;
    const char* catsprite = "(^._.^)"

    while (r){

        int ch = getch();
        
        if (ch == 'q'){
            r = 0;
        }
        if ((ch == 'a' || ch == KEY_LEFT) && Xpl > 3){
            Xpl--;
        }
        if ((ch == 'd' || ch == KEY_RIGHT) && Xpl < Xborder )


    }

}