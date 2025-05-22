#include <ncurses.h>
#include "shop.h"
#include "globals.h"

int shop(int c, int cols, int rows)
{   
    flushinp();
    clear();

    do
    {   

        // TODO - random shop includings
        attron(A_BOLD);
        mvprintw(1,rows / 2 - 16,"Welcome to the secret shop!");
        attroff(A_BOLD);

        mvprintw(6,0, "\t\t\t'1' to buy healing salve (50 gold) ");
        mvprintw(7,0, "\t\t\t'2' to buy 1 att (60 gold) ");
        mvprintw(8,0, "\t\t\t'3' to buy obsidian sword (150 gold) ");
        mvprintw(9,0, "\t\t\t'4' to buy armor (200 gold) ");
        mvprintw(cols - 1, rows / 2 - 8, "'ESC' to leave. ");
        mvprintw(cols /2 , rows /2 - 25, "HP: %d \t Armor: %d \t Att: %d \t" "Gold: %d \t Dlvl: %d", hp, armor * 20, att, p_gold, dlvl);

        c = getch();

        // shop functions implementation
        if (c == '1' && p_gold >= 50)
        {   
            p_gold -= 50;
            hp += 10;
        } else if (c == '2' && p_gold >= 60)
        {
            p_gold -= 60;
            att++;
        } else if (c == '3' && p_gold >= 150)
        {
            p_gold -= 150;
            att+= 3;
        } else if (c == '4' && p_gold >= 200 && armor < MAX_ARMOR)
        {
            p_gold -= 200;
            armor++;
        }

    } while (c != 27 && c != '\n' && c != KEY_ENTER && c != ' ');

    refresh();
    return 0;
}