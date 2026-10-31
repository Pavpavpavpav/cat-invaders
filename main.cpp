#include <curses.h>
#include <vector>
#include <string>
#include <cstdio>

struct catsprite {
    std::vector<std::string> sprite;

};

struct bullet {
    int x, y;
    int dx;
};

struct mouse {
    // for when I make the enemies
};

struct powerup {
    int x, y;
    int type;

    // 1 -> spreadshot
    // 2 -> double points (wip)
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
          "=^! !^=",
        }
    };

    int currsprite = 0;

    int Xborder, Yborder;
    getmaxyx(stdscr, Yborder, Xborder);

    if (Yborder < 15 || Xborder < 25){
        endwin();
        printf("Terminal too small: please increase the window size!\n" );
        return 1;
    }

    int Xpl = Xborder / 2;
    int Ypl = Yborder - 4;


    bool r = true;
    int blinktimer = 160;

    std::vector<bullet> bullets;
    int btick = 0;
    int fireface = 0;

    std::vector<powerup> powerups;
    int ptick = 0;
    int pface = 0;
    int poweruptime = 0;
    int powerup;

    while (r){
        blinktimer--;
        
        if (poweruptime > 0){
            poweruptime--;
        }

        if (blinktimer > 155){
            currsprite = 1;
        }
        else if (blinktimer == 0){
            blinktimer = 160;
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
        if (ch == ' ' && bullets.size() < 2){
            bullets.push_back({Xpl, Ypl - 1});
            fireface  = 10;
        }
        if (ch == '1' && powerups.size () < 1) {
            powerups.push_back({Xborder / 2, 1, 1});
        }

        if (pface > 0){
            currsprite = 4;
            pface--;
        }

        if (fireface > 0){
            currsprite = 3;
            fireface--;
        }

        ptick++;
        if (ptick > 6){
            ptick = 0;
            for (int i = 0; i < powerups.size(); i++){
                powerups[i].y++;
            }

            for (int i = powerups.size() - 1; i >= 0; i--){
                if (powerups[i].y == Ypl && (powerups[i].x >= Xpl - 3 && powerups[i].x <= Xpl + 3)){
                    powerup = 1;
                    pface = 10;
                    poweruptime = 170;
                    powerups.erase(powerups.begin() + i);
                }
            }

            for (int i = powerups.size() - 1; i >= 0; i--){
                if (powerups[i].y > Ypl){
                    powerups.erase(powerups.begin() + i);
                }
            }
        }


        btick++;
        if (btick > 5){
            btick = 0;
            for (int i = 0; i < bullets.size(); i++){
                bullets[i].y--;
            }

            for (int i = bullets.size() - 1; i >= 0; i--){
                if (bullets[i].y < 0){
                    bullets.erase(bullets.begin() + i);
                }
            }
        }



        erase();
        mvprintw(Ypl, Xpl - 3, "%s", cat.sprite[currsprite].c_str());
        for (int i = 0; i < bullets.size(); i++){
            mvprintw(bullets[i].y, bullets[i].x, "^");
        }
        for (int i = 0; i < powerups.size(); i++){
            mvprintw(powerups[i].y, powerups[i].x, "#");
        }
        mvprintw(0, 0, "bullets=%d/2 powertime=%d", (int)bullets.size(), poweruptime/32);
        wnoutrefresh(stdscr);
        doupdate();

        napms(32);
    }

    endwin();
    return 0;
}