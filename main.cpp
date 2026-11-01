#include <curses.h>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>

//Hi! If u are trying to edit this code to tweak my game feel free to do so! I have left some comments that can explain which part of the code is what, but you might still have to dig a bit cos it is not the most optimized :P

struct catsprite {
    std::vector<std::string> sprite;

};

struct bullet {
    int x, y;
    int dx;
};

struct mouse {
    int x, y;
    bool alive;
};

struct powerup {
    int x, y;
    int type;

    // 1 -> spreadshot
    // 2 -> frenzy
    // 3 -> double points (wip)
    // 4 -> bomb
};

struct explosion {
    int x,y;
    int timer;
    int cdel;
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

    srand(time(0));

    int currsprite = 0;

    int Xborder, Yborder;
    getmaxyx(stdscr, Yborder, Xborder); //window/termial broder

    if (Yborder < 15 || Xborder < 25){
        endwin();
        printf("Terminal too small: please increase the window size!\n" );
        return 1;
    }

    int Xpl = Xborder / 2;
    int Ypl = Yborder - 4;

    int lives = 3; //# of bombs the cat can withstanc
    int hurtface = 0;

    bool r = true;
    int blinktimer = 160;

    float cooldown = 0;

    std::vector<bullet> bullets;
    int btick = 0;
    int fireface = 0;

    std::vector<powerup> powerups;
    int ptick = 0;
    int pface = 0;
    int poweruptime = 0;
    int powerup = 0;

    std::vector<explosion> boom;
    int bombblink = 31;
    int bobmtype = 0;

    int maxbullets[] = { 2, 6, 4, 2};
    const char* powername[] { "none", "Spreadshot", "Frenzy", "Double points"};

    std::vector<mouse> mice;
    for (int row = 0; row < 4; row++){
        for (int col = 0; col < 8; col++){
            mice.push_back({4 + col * 5, 3 + row * 2, true});
        }
    }
    int dir = 1;
    int mtick = 0;
    int s = 0;
    bool w = false;

    while (r){
        blinktimer--;

        if (lives <= 0){
            r = false;
        }
        
        if (bombblink > 16){
            bobmtype = 0;
            bombblink--;
        }
        else if (bombblink <= 16 && bombblink > 0) {
            bobmtype = 1;
            bombblink--;
        }
        else {
            bombblink = 31;
        }

        if (poweruptime > 0){
            poweruptime--;
        }
        else {
            powerup = 0;
        }

        if (cooldown > 0){
            cooldown--;
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
        

        //keypresses and what they do belows VVV
        if (ch == 'q'){ 
            r = 0;
        }
        if ((ch == 'a' || ch == KEY_LEFT) && Xpl > 3){
            Xpl--;
        }
        if ((ch == 'd' || ch == KEY_RIGHT) && Xpl < Xborder - 4){
            Xpl++;
        }
        if (ch == ' ' && cooldown == 0){
            switch (powerup){
                case 1:
                    if (bullets.size() <= 5){
                    bullets.push_back({Xpl, Ypl - 1, -1});
                    bullets.push_back({Xpl, Ypl - 1, 0});
                    bullets.push_back({Xpl, Ypl - 1, 1});
                    fireface = 10;
                    cooldown = 32;
                    }
                    break;
                case 2: 
                    if (bullets.size() <= 3){
                        bullets.push_back({Xpl, Ypl - 1, 0});
                        fireface = 10;
                        cooldown = 8;
                    }
                    break;
                default:
                    if (bullets.size() <= 1){
                        bullets.push_back({Xpl, Ypl - 1, 0});
                        fireface = 10;
                        cooldown = 32;
                    }
                    break;
            }
        }
        if (ch == '1') {
            powerups.push_back({Xpl, 1, 1});
        }

        if (ch == '2') {
            powerups.push_back({Xpl, 1, 2});
        }
        if (ch == '3'){
            powerups.push_back({Xpl, 1, 3});
        }
        if (ch == '4') {
            powerups.push_back({Xpl, 1, 4});
        }

        if (hurtface > 0){
            currsprite = 2;
            hurtface--;
        }

        if (pface > 0){
            currsprite = 4;
            pface--;
        }

        if (fireface > 0){
            currsprite = 3;
            fireface--;
        }

        ptick++; //powerup logic
        if (ptick > 6){
            ptick = 0;
            for (int i = 0; i < powerups.size(); i++){
                powerups[i].y++;
            }

            for (int i = powerups.size() - 1; i >= 0; i--){
                if (powerups[i].y == Ypl && (powerups[i].x >= Xpl - 3 && powerups[i].x <= Xpl + 3)){
                    switch (powerups[i].type){
                        case 4:
                            boom.push_back({powerups[i].x, powerups[i].y, 16, 0});
                            break;
                        default:
                            powerup = powerups[i].type;
                            pface = 10;
                            poweruptime = 170;
                            if (powerup == 3){
                                s+= 200;
                            }
                            else {
                                s += 100;
                            }
                            break;  
                    }
                    powerups.erase(powerups.begin() + i);
                }
            }

            for (int i = powerups.size() - 1; i >= 0; i--){
                if (powerups[i].y > Ypl){
                    powerups.erase(powerups.begin() + i);
                }
            }
        }


        btick++; //bulet logic
        if (btick > 5){
            btick = 0;
            for (int i = 0; i < bullets.size(); i++){
                bullets[i].y--;
                bullets[i].x += bullets[i].dx;
            }

            for (int i = bullets.size() - 1; i >= 0; i--){
                if (bullets[i].y < 0){
                    bullets.erase(bullets.begin() + i);
                    continue;
                }

                for (int p = powerups.size() - 1; p >= 0; p--){
                    if (powerups[p].type == 4 && powerups[p].x == bullets[i].x && abs(powerups[p].y - bullets[i].y) <= 1){
                        boom.push_back({powerups[p].x, powerups[p].y, 16, 0});
                        if (powerup == 3){
                            s += 30;
                        }
                        else {
                            s += 15;
                        }
                        bullets.erase(bullets.begin() + i);
                        powerups.erase(powerups.begin() + p);
                        break;
                    }
                }
            }

            for (int i = bullets.size() - 1; i >= 0; i--){
                for (int m = 0; m < mice.size(); m++){
                    if (mice[m].alive == false){
                        continue;
                    }
                    if (abs(mice[m].y - bullets[i].y) <= 1 && bullets[i].x >= mice[m].x && bullets[i]. x <= mice[m].x + 2){
                        mice[m].alive = false;
                        if (powerup == 3){
                            s += 100;
                        }
                        else {
                            s += 50;
                        }
                        if (rand() % 8 == 0){
                            powerups.push_back({mice[m].x + 1, mice[m].y + 1, 1 + (rand() % 3)});
                        }
                        bullets.erase(bullets.begin() + i);
                        break;
                    }
                }
            }
        }



        for (int k = 0; k < boom.size(); k++){ // bomb explosion logic
            if (boom[k].cdel > 0 || boom[k].timer != 16){
                continue;
            }

            int x = boom[k].x; 
            int y = boom[k].y;  

            for (int i = bullets.size() - 1; i >= 0; i--){
                if (abs(x - bullets[i].x) <= 2 && abs(y - bullets[i].y) <= 2 ) {
                    bullets.erase(bullets.begin() + i);
                }
            }

            for (int i = powerups.size() - 1; i >= 0; i--){
                if (abs(x - powerups[i].x) <= 2 && abs(y - powerups[i].y) <= 2 ){
                    if (powerups[i].type == 4){
                        boom.push_back({powerups[i].x, powerups[i].y, 16, 10});
                        if (powerup == 3){
                            s += 10;
                        }
                        else {
                            s += 5;
                        }
                    }
                    powerups.erase(powerups.begin() + i);
                }
            }

            if (abs(y - Ypl) <= 2 && Xpl + 3 >= x - 2 && Xpl - 3 <= x + 2){
                lives--;
                s -= 250;
                hurtface = 15;
            }

            for (int i = 0; i < mice.size(); i++){
               if (mice[i].alive == false){
                    continue;
               } 
               if (abs(y - mice[i].y) <= 2 && mice[i].x + 3 >= x - 2 && mice[i].x - 3 <= x + 2){
                    mice[i].alive = false;
                    if (powerup == 3){
                        s += 100;
                    }
                    else {
                        s += 50;
                    }
                    if (rand() % 8 == 0){
                        powerups.push_back({mice[m].x + 1, mice[m].y + 1, 1 + (rand() % 3)});
                    }
               }
            }
        }

        for (int i = boom.size() - 1; i >= 0; i--){
            if (boom[i].cdel > 0){
                boom[i].cdel--;
            }
            else {
                boom[i].timer--;
                if (boom[i].timer < 1){
                    boom.erase(boom.begin() + i);
                }
            }
        }


        mtick++;

        std::vector<int> alive; // mice drop bobm
        if (mtick > 3){
            alive.clear();
            for (int i = 0; i < mice.size(); i++){
                if (mice[i]. alive == true){
                    alive.push_back(i);
                }
            }
            for (int i = 0; i < alive.size(); i++){
                if (rand() % 501 == 0){
                    powerups.push_back({mice[alive[i]].x, mice[alive[i]].y, 4});
                }
            }
        }

         //mous formation movement heres VVV
        if (mtick > 5 + alive.size()/3){
            mtick = 0;
            bool hitedg = false;
            for (int i = 0; i < mice.size(); i++){
                if (mice[i].alive != true){
                    continue;
                }
                if (dir == 1 && mice[i].x + 3 >= Xborder - 1){
                    hitedg = true;
                }
                if (dir == -1 && mice[i].x <= 1){
                    hitedg = true;
                }
            }
            for (int i = 0; i < mice.size(); i++){
                if (hitedg == true){
                    mice[i].y++;
                }
                else {
                    mice[i].x += dir;
                }
            }
            if (hitedg == true){
                dir = -dir;
            }


        }


        erase(); //printing the actuall stuffs such as the mice and the cat and the powerups and the bombs on the screen
        mvprintw(Ypl, Xpl - 3, "%s", cat.sprite[currsprite].c_str());
        for (int i = 0; i < bullets.size(); i++){
            mvprintw(bullets[i].y, bullets[i].x, "^");
        }
        for (int i = 0; i < powerups.size(); i++){
            switch(powerups[i].type) {
                case 4:
                    if (bobmtype == 1){
                        mvprintw(powerups[i].y, powerups[i].x, "@");
                    }
                    else {
                        mvprintw(powerups[i].y, powerups[i].x, "O");
                    }
                    break;
                default:
                     mvprintw(powerups[i].y, powerups[i].x, "#");
            }
        }

        for (int i = 0; i < boom.size(); i++){
            if (boom[i].cdel > 0){
                mvprintw(boom[i].y, boom[i].x, bobmtype == 1 ? "@" : "O" );
            }
            else {
                for (int y = -2; y <= 2; y++){
                    mvprintw(boom[i].y + y, boom[i].x-2, "*****");
                }
            }
        }

        for (int i = 0; i < mice.size(); i++){
            if (mice[i].alive == true){
                mvprintw(mice[i].y, mice[i].x, dir == 1 ? "~o>" : "<o~");
            }
        }
        mvprintw(0, 0, "bullets=%d/%d cooldown=%.1f powertime=%d power=%s", (int)bullets.size(), maxbullets[powerup], cooldown/31, poweruptime/31, powername[powerup]);
        mvprintw(1, 0, "lives=%d/3 score=%d", lives, s);
        wnoutrefresh(stdscr);
        doupdate();

        napms(32);
    }

    endwin();
    return 0;
}