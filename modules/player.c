#include <ncurses.h>
#include <stdlib.h>
#include "player.h"
#include "globals.h"
#include "shop.h"
#include "faq.h"


int respawn_player(int rows, int cols, char (* map) [cols])
{   
    if (!p_placed)
    {   
        int dist_y;
        int dist_x;
        int max_attempts = 1000;
        int attempts = 0;

        while (attempts < max_attempts)
        {
            py = rand() % rows;
            px = rand() % cols;

            if (py >= rows - 1|| px >= cols - 1) continue;

            // distance from stairs
            dist_y = abs(py - sy);
            dist_x = abs(px - sx);

            if (map[py][px] == ' ' && dist_y <= 5 + dlvl / 4 && dist_x <= 5 + dlvl / 4)
            {
                break;
            }

            attempts++;
        }

        if (attempts >= max_attempts)
        {
            do
            {
                py = rand() % rows;
                px = rand() % cols;
            }
            while (py >= rows - 1 || px >= cols - 1|| map[py][px] != ' ');
            mvprintw(0, 0, "Fallback spawn for player on level %d", dlvl); // debug
            refresh();
        }
            
        p_placed = 1;
    }
      return 0;
}

int p_action(int c, int cols, int rows, char (* map) [cols])
{
    int dir_y = py, dir_x = px;

    // wasd remap
    if      (c == 'w')
        c = KEY_UP;
    else if (c == 's')
        c = KEY_DOWN;
    else if (c == 'a')
        c = KEY_LEFT;
    else if (c == 'd')
        c = KEY_RIGHT;

    // arrows
    if (c == KEY_UP) dir_y--;
    else if(c == KEY_DOWN) dir_y++;
    else if(c == KEY_LEFT) dir_x--;
    else if(c == KEY_RIGHT) dir_x++;

    else if (c == '?')
    {   
        flushinp();
        clear();
        faq(rows, cols);
    }

    // go deeper
    else if ((c == '>'|| c == '\n' || c == KEY_ENTER || c == ' ') && map[py][px] == '>')
    {   
        mvprintw(0, 0, "Transitioning to level %d", dlvl + 1); // debug
        refresh();

        s_placed = 0;
        p_placed = 0;
        r_placed = 0;

        return 1;
    }

    // shop
    else if ((c == '$'|| c == '\n' || c == KEY_ENTER || c == ' ') && map[py][px] == '$')
    {   
        shop(c, rows, cols);   
    }
    
    // allow pass through 
    if (map[dir_y][dir_x] == ' ' || map[dir_y][dir_x] == '>' || map[dir_y][dir_x] == '$')
    {
        py = dir_y;
        px = dir_x;
    }

    // battle
    else if (map[dir_y][dir_x] == 's')
    {
        battle(cols, map, dir_y, dir_x);
    }

    return 0;
}

int battle(int cols, char (* map) [cols], int dir_y, int dir_x)
{
    for(int m = 0; m < 10; m++)
    {
        if (dir_y == monster[m].y && dir_x == monster[m].x)
        {   

            // new battle
            monster[m].lvl -= att;
            show_attack_message = 1;

            if (monster[m].lvl <= 0)
            {   
                if (monster[m].elite)
                {
                    p_gold += 20; 
                    xp += 50; 
                }

                if (monster[m].boss)
                {
                    p_gold += 50; 
                    xp += 100; 
                }

                if (monster[m].red && rand() % 2 )
                {
                    hp += rand() % dlvl + 3;
                    show_heal_message = 1;
                }

                if (monster[m].yellow && rand() % 2)
                {
                    hp += rand() % dlvl + 4;
                    show_heal_message = 1;

                }

                if (monster[m].white && rand() % 2)
                {
                    hp += rand() % dlvl + 5;
                    show_heal_message = 1;
                }

                show_kill_message = 1;
                s_defeated++;
                map[dir_y][dir_x] = ' ';
                p_gold += rand() % 10 + 1;

                int base_xp = 5 + dlvl * 2;
                int rand_xp = rand() % 5;
                int type_bonus = abs(monster[m].type - 97) / 2;
                xp += base_xp + rand_xp + type_bonus;

                //lvl up check
                if (xp >= xp_to_next_level)
                {   
                    xp -= xp_to_next_level;
                    p_lvl ++;
                    hp += 5 + dlvl / 2;
                    att += 1;
                    xp_to_next_level = p_lvl * 100;
                }
            }
            break;
        }
    }
    return 0;
}