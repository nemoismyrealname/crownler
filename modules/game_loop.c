#include <ncurses.h>
#include "game_loop.h"
#include "globals.h"
#include "dungeon.h"
#include "enemies.h"
#include "player.h"
#include "render.h"

int game_loop(int c, int rows, int cols, char (* map) [cols])
{
    bool new_lvl = 0;
    static bool first_run = 1; 

    if (first_run || r_placed == 0 )
    {
        mvprintw(0, 0, "Generating level %d...", dlvl); // debug
        refresh();

        if (dungeon_gen(rows, cols, map) != 0)
        {
            mvprintw(0, 0, "Error generating level %d. Press any key to retry.", dlvl);
            refresh();
            getch();
            return c; 
        }

        if (respawn_player(rows, cols, map) != 0 || respawn_enemies(rows, cols, map) != 0)
        {
            mvprintw(0, 0, "Error spawning entities on level %d. Press any key to retry.", dlvl);
            refresh();
            getch();
            return c;
        }
        first_run = 0;
    }

    if (c != 0)
    {
        new_lvl = p_action(c, cols, rows, map);
    }

    monster_turn(rows, cols, map);
    dungeon_draw(rows, cols, map);

    attron(A_BOLD);
    mvaddch(py,px, '@'); // draw playa
    attroff(A_BOLD);
    
    if (show_attack_message) {

        // attron(COLOR_PAIR(RED));
            mvprintw(0,0, "\t\t\t\t\t> You have attacked an enemy");
        // attroff(COLOR_PAIR(RED));

        show_attack_message = 0;
    }

    if (show_kill_message) {

        // attron(COLOR_PAIR(GREEN));
            mvprintw(0,0, "\t\t\t\t\t\t\t\t\t\t>> You've killed an enemy");
        // attroff(COLOR_PAIR(GREEN));
        
        show_kill_message = 0;
    }

    if (show_damage_message) {

        attron(COLOR_PAIR(RED));
            mvprintw(0,cols - 6, "HP-");
        attroff(COLOR_PAIR(RED));
        
        show_damage_message = 0;
    }


    if (show_miss_message) {

        attron(COLOR_PAIR(CYAN));
            mvprintw(0,0, "\t> You've dodged enemy attack");
        attroff(COLOR_PAIR(CYAN));
        
        show_miss_message = 0;
    }

    if (show_heal_message) {

        attron(COLOR_PAIR(GREEN));
            mvprintw(0,0, "   HP+");
        attroff(COLOR_PAIR(GREEN));
        
        show_heal_message = 0;
    }

    if (new_lvl)
    {   
        mvprintw(0, 0, "Level %d is loaded. (Press any button to continue)", ++dlvl);
    }
    
    if (hp < 1)
    {
        clear();
        mvprintw(rows /2, cols/2 - 5 , "YOU DIED.\n\n\n\n\n\n\n\n\n\n\n");
        printw("\n > 'Enemies defeated: %d\n", s_defeated);
        printw("\n > 'ESC' to quit");
        printw("\n > 'ENTER/SPACE' to restart");
        refresh();

        while (1)
        {
            flushinp();
            c = getch();
            if (c == 27) return 27;
            else if (c == ' ' || c == '\n')
            {
                att = 1;
                hp = 15;
                p_gold = 0;
                dlvl = 1;
                xp = 0;
                p_lvl = 1;
                s_placed = 0;
                p_placed = 0;
                r_placed = 0;
                s_defeated = 0;
                armor = 0;
                first_run = 0;

                for (int m = 0; m < 10; m++) {
                    monster[m].y = 0;
                    monster[m].x = 0;
                    monster[m].lvl = 0;
                    monster[m].type = 0;
                    monster[m].awake = 0;
                    monster[m].red = 0;
                    monster[m].yellow = 0;
                    monster[m].white = 0;
                    monster[m].elite = 0;
                    monster[m].boss = 0;
                }

                dungeon_gen(rows, cols, map);
                respawn_enemies(rows, cols, map);
                respawn_player(rows, cols, map);

                clear();
                mvprintw(rows /2, cols /2 - 25, "Welcome back to level %d. (Press any button to continue)", dlvl);
                break;
            }
        }
    }

    refresh();
    c = getch();
    return c;
}