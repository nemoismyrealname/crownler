#include <ncurses.h>
#include "globals.h"
#include "faq.h"

FAQEntry* (*get_faq_data[])() = {NULL}; // empty arr
int faq_data_count = 0;

void faq(int rows, int cols)
{   
    clear();
    attron(A_BOLD);
    mvprintw(1, cols / 2 - 8, "=== FAQ and Stats ===");
    attroff(A_BOLD);

    int current_line = 3;
    for (int i = 0; i < faq_data_count && current_line < rows - 2; i++)
    {
        if (get_faq_data[i] != NULL) {

            FAQEntry *entry = get_faq_data[i]();

            attron(A_BOLD | COLOR_PAIR(YELLOW));
            mvprintw(current_line, 2, "%s:", entry->label);
            attroff(A_BOLD | COLOR_PAIR(YELLOW));

            mvprintw(current_line + 1, 4, "%s", entry->value);

            int lines = 1;

            for (const char *p = entry->value; *p; p++)
            {
                if (*p == '\n') lines++;
            }
            
            current_line += lines + 1;
        }
    }

    mvprintw(rows - 1, 0, "Press 'ESC', 'Enter' or 'Space' to exit.");
    refresh();

    int c;
    do {
        c = getch();
    } while (c != 27 && c != '\n' && c != ' ' && c != KEY_ENTER);

    clear();
}