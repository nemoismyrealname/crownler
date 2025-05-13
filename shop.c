#include <ncurses.h>
#include "shop.h"
#include "globals.h"

int shop(int c, int cols, int rows)
{   
    flushinp();
    clear();

    do
    {
        attron(A_BOLD);
        mvprintw(1,rows / 2 - 16,"Welcome to the secret shop!");
        attroff(A_BOLD);

        mvprintw(6,0, "\t\t\t'1' to buy healing slave (10 gold) ");
        mvprintw(7,0, "\t\t\t'2' to buy 1 att (10 gold) ");
        mvprintw(8,0, "\t\t\t'3' to buy obsidian sword ");
        mvprintw(9,0, "\t\t\t'4' to buy armor ");
        mvprintw(cols - 1, rows / 2 - 8, "'ESC' to leave. ");
        mvprintw(cols /2 , rows /2 - 25, "HP: %d \t Att: %d \t" "Gold: %d \t Dlvl: %d",hp, att, p_gold, dlvl);

        c = getch();

        // shop functions implementation
        if (c == '1' && p_gold >= 10)
        {   
            p_gold -= 10;
            hp += 10;
        } else if (c == '2' && p_gold >= 10)
        {
            p_gold -= 10;
            att++;
        }

    } while (c != 27 && c != '\n' && c != KEY_ENTER && c != ' ');

    refresh();
    return 0;
}