#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
// #include <stdbool.h>
#include "game_loop.h"
#include "globals.h"

int main (void)
{
    int c = 0; 
    int rows, cols;
    srand(time(NULL));

    initscr();
    start_color();
    use_default_colors();

    // init color pairs -> 1st atr linked with color const //
    init_pair (RED, COLOR_RED, COLOR_BLACK);
    init_pair (GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair (YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair (BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair (MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair (CYAN, COLOR_CYAN, COLOR_BLACK);

    keypad(stdscr, 1); // allow arrows, F1-F12
    noecho(); // no input echo
    curs_set(0); // hide cursor

    getmaxyx(stdscr, rows, cols);

    char map[rows][cols]; // 0 - 119

    do
    { 
        c = game_loop(c, rows - 1, cols, map); // rows - 1 cause 0 - 119
    } while (c != 27); // 27 ASCII = ESC

    refresh();
    endwin();
    return 0;
}