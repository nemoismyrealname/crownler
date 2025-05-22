#include <ncurses.h>
#include "render.h"
#include "globals.h"

int dungeon_draw(int rows, int cols, char (* map) [cols])
{
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {      
            if (y == 0 || y == rows)
                mvaddch(y, x, ' ');

            else if (map[y][x] == '%')
                mvaddch(y, x, '%');

            else if (map[y][x] == '>')
            {
                attron(A_BOLD);      
                    mvaddch(y, x, '>');
                attroff(A_BOLD); 
            }

            else if (map[y][x] == '$')
            {
                attron(A_BOLD);      
                    mvaddch(y, x, '$');
                attroff(A_BOLD); 
            }
                   
            else if (map[y][x] == ' ')
                mvaddch(y, x, ' ');

            else if (map[y][x] == '#')
                mvaddch(y, x, '#');
        
            else if (map[y][x] == 's')
            {   
                for (int m = 0; m < 10; m++)
                {
                    if (monster[m].y == y && monster[m].x == x)
                    {   
                        if (monster[m].elite)
                        {
                            attron(COLOR_PAIR(BLUE));
                                mvaddch(y, x, monster[m].type);
                        }

                        else if (monster[m].boss)
                        {
                            attron(COLOR_PAIR(CYAN));
                                mvaddch(y, x, monster[m].type);
                        }
                    
                        else if (monster[m].lvl < dlvl / 2 + 2)
                        {
                            attron(COLOR_PAIR(RED));
                                mvaddch(y, x, monster[m].type);
                        }
                        else if (monster[m].lvl < dlvl + 2)
                        {
                            attron(COLOR_PAIR(YELLOW));
                                mvaddch(y, x, monster[m].type);
                        }
                        else 
                            mvaddch(y, x, monster[m].type);
                    }
                    standend();
                }
            }
        }
    }

    mvprintw(rows, 0, "HP: %d \t Armor: %d (%%) \t Lvl: %d (%d / %d) \t Att: %d \t" "Gold: %d \t Dlvl: %d",hp, armor * 20,  p_lvl, xp, xp_to_next_level, att, p_gold, dlvl);
    return 0;
}