#include <ncurses.h>
#include <stdlib.h>
#include "player.h"
#include "globals.h"
#include "shop.h"

int respawn_player(int rows, int cols, char (* map) [cols])
{   
    if (!p_placed)
    {   
        int dist_y;
        int dist_x;
        
        do
        {
            py = rand() % rows;
            px = rand() % cols;
            
            // distance from stairs
            dist_y = abs(py - sy);
            dist_x = abs(px - sx);
            
            if (map[py][px] == ' ' && (dist_y > 7 + dlvl / 2 || dist_x > 7 + dlvl / 2)) break;

        } while (1);
        
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

    // go deeper
    else if ((c == '>'|| c == '\n' || c == KEY_ENTER || c == ' ') && map[py][px] == '>')
    {
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
            
            if (monster[m].lvl <= 0)
            {   
                show_kill_message = 1;
                s_defeated++;
                map[dir_y][dir_x] = ' ';
                p_gold += rand() % 10 + 1;

                if (rand() % 4)
                    hp += rand() % dlvl + 1; // heal on kill chance - TODO life stealing mask

                if (rand() % 2) // TODO - persona attack upgrade
                    att++;
           
                // C4S1N0
                if ((dlvl == 1 && !(rand() % 50) && s_defeated < 4)) // 20% for first 4 enemies
                {
                    att += 5;
                    hp += 20;
                    p_gold += 100;
                    show_casino_message = 1;
                }
            }

            break;
        }
    }
    return 0;
}