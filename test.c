#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// color const
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6

int sy, sx;
int py, px; 
int att = 1;
int hp = 10;
bool s_placed = 0;
bool p_placed = 0;
int r_placed = 0;
int p_gold = 0;
int dlvl = 1;
int s_defeated = 0;
bool show_casino_message = 0;
bool show_shop_message = 0;
bool show_kill_message = 0;
bool show_damage_message = 0;
int session = 0;


struct monsters
{
    int y;
    int x;
    int lvl;
    int type;
    bool awake;
};

struct monsters monster[10];

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
                for (int m =0; m < 10; m++)
                {
                    if (monster[m].y == y && monster[m].x == x)
                    {   
                        if (monster[m].lvl < dlvl / 2 + 2)
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

    mvprintw(rows, 0, "HP: %d \t Att: %d \t" "Gold: %d \t Dlvl: %d",hp, att, p_gold, dlvl);
    return 0;
}

int monster_turn(int rows, int cols, char (* map) [cols])
{
   int dist_y, dist_x;

    for(int m = 0; m < 10; m++)
        {   
            if (monster[m].lvl < 1)
                continue;

            dist_y = abs(monster[m].y - py);
            dist_x = abs(monster[m].x - px);

            if (dist_y < 3 && dist_x < 3)
                monster[m].awake = 1;

            if (monster[m].awake == 0)
                continue;

            int dir_y = monster[m].y;
            int dir_x = monster[m].x;

            //std Y-X walk
            if (dist_y > dist_x)
            {
                if (dir_y > py)
                    dir_y -=1;
                else dir_y +=1;
            }
            else 
            {
                if (dir_x > px)
                    dir_x -= 1;
                else dir_x += 1;
            }

            //in failure - diag corner walk
            if (map[dir_y][dir_x] == '#' || map[dir_y][dir_x] == '%' || map[dir_y][dir_x] == '>')
            {   
                dir_y = monster[m].y;
                dir_x = monster[m].x;

                if (dir_y > py)
                    dir_y -= 1;
                else dir_y +=1;

                if (dir_x > px)
                    dir_x -=1;
                else dir_x +=1;
            }
            
            //in failure x2 - wall bumping remove
            if (map[dir_y][dir_x] == '#' || map[dir_y][dir_x] == '%' || map[dir_y][dir_x] == '>')
            {
                    dir_y = monster[m].y;
                    dir_x = monster[m].x;

                    if (dist_y > dist_x)
                    {
                        if (dir_x > px)
                            dir_x -=1;
                        else dir_x +=1;
                    }
                else 
                {
                    if (dir_y > py)
                        dir_y -=1;
                    else dir_y +=1;
                }
            }
            


            if (dist_y < 2 && dist_x < 2)
            {
                hp -= dlvl /2 + 1;
                show_damage_message = 1;
            }
            

            else if (map[dir_y][dir_x] == ' ')
            {   
                map[monster[m].y][monster[m].x] = ' ';

                monster[m].y = dir_y;
                monster[m].x = dir_x;
                map[monster[m].y][monster[m].x] = 's';
            }
        }

    return 0;
}

int battle(int cols, char (* map) [cols], int dir_y, int dir_x)
{
    for(int m = 0; m < 10; m++)
    {
        if (dir_y == monster[m].y && dir_x == monster[m].x)
        {       
            monster[m].lvl -= att;

            // new battle
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

            // if (monster[m].lvl <= 0)
            // {   
            //     show_kill_message = 1;
            //     s_defeated ++;
            //     map[dir_y][dir_x] = ' ';
            //     p_gold += rand() % 10 + 1;

            //     if(rand() % 2)
            //     hp += rand() % dlvl + 1; //heal on kill chance

            //     if(rand() % 2)
            //     att++;

            //     //C4S1N0
            //     if((dlvl == 1 && !(rand() % 50) && s_defeated < 4)) // 20% for first 4 enemies
            //     {
            //         att += 8;
            //         hp += 23;
            //         p_gold += 101;
            //         show_casino_message = 1;
            //     }
            // }
               
            // else
            // {
            //     monster[m].lvl -= att;
            // }
                
            break;
        }
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

int respawn_ent(int rows, int cols, char (* map) [cols])
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

    if (!s_placed)
    {   
        int my, mx;
        int max_attempts = 1000;
        
        for(int m = 0; m < 10; m++)
        {   
            int attempts = 0;
            bool placed = 0;

            do {
                my = rand() % rows;
                mx = rand() % cols;

                int dist_y = abs(my - py);
                int dist_x = abs(mx - px);
                
                if (map[my][mx] == ' ' && dist_y >= 3 && dist_x >= 3)
                {
                    placed = true;
                    break;
                }

                attempts++;

                if (attempts >= max_attempts)
                {
                    // less player detect field
                    do
                    {
                        my = rand() % rows;
                        mx = rand() % cols;
                    } 
                    while (map[my][mx] != ' ');
                    placed = true;
                    break;
                } 

            } while (1);

            monster[m].y = my;
            monster[m].x = mx;

            monster[m].lvl = rand() % dlvl + 2;

            //red
            if (dlvl == 1 && !(rand() % 5))
                monster[m].lvl = 1;

            //white
            if (rand() % 2)
                monster[m].lvl = dlvl + 2;

            monster[m].type = rand() % dlvl + 97;

            //yellow
            if (dlvl == 1 && !(rand() % 3))
                monster[m].type += 1;

            monster[m].awake = 0;

            map[monster[m].y][monster[m].x] = 's';
        }
        s_placed = 1;

    }

    }
      return 0;
}

int dungeon_gen(int rows, int cols, char (* map) [cols]) 
{
    if(!r_placed)
    {   
        int ry, rx; 
        int r_size_y, r_size_x;
        int r_center_y, r_center_x;
        int r_old_center_y, r_old_center_x;
        int room_num = rand() % 5 + 5; // room number
        bool collision;

        // dg walls and border
        for(int y = 0; y < rows; y++)
        {
            for(int x = 0; x < cols; x++)
            {      
                if(y == 0 || y == 1 || y == rows - 1 || x == 0 || x == cols - 1 || y == rows)
                {
                    map[y][x] = '%'; // borders
                }
                else {
                    map[y][x] = '#'; // walls
                } 
            }
        }

        do
        {
                // no collision check
            do 
            {   
                collision = 0;

                //gen room coords
                ry = rand() % (rows - 4) + 1;
                rx = rand() % (cols - 4) + 1;

                // room size
                r_size_y = rand() % 5 + 4;
                r_size_x = rand() % 15 + 8;

                // fill DB map with rooms
                for(int y = ry; y <= ry + r_size_y; y++)
                {
                    for(int x = rx; x <= rx + r_size_x; x++)
                    {   
                        if (map[y][x] == '%' || map[y][x] == ' ' 
                            || map[y + 2][x] == ' ' || map[y - 2][x] == ' ' 
                            || map[y][x + 2] == ' ' || map[y][x - 2] == ' ')
                        {   
                            collision = 1;
                            y = ry + r_size_y;
                            break;
                        }
                        
                    }
                }

            } while (collision == 1);

                // fill DB map with rooms
            for(int y = ry; y <= ry + r_size_y; y++)
            {
                for(int x = rx; x <= rx + r_size_x; x++)
                {   
                    if(map[y][x] != '%')
                        map[y][x] = ' ';
                }
            }
                r_placed++;

                // tunnels
                if(r_placed > 1)
                {
                    r_old_center_y = r_center_y;
                    r_old_center_x = r_center_x;
                }
               
                r_center_y = ry + (r_size_y /2) ;
                r_center_x = rx + (r_size_x /2) ;

                if(r_placed > 1)
                {   
                    int path_y;
                    for(path_y = r_old_center_y; path_y != r_center_y; )
                    {
                        map[path_y][r_old_center_x] = ' ';
                        if(r_old_center_y < r_center_y) path_y++;
                        else if (r_old_center_y > r_center_y) path_y--;
                    }

                    for(int x = r_old_center_x; x != r_center_x; )
                    {
                        map[path_y][x] = ' ';
                        if(r_old_center_x < r_center_x) x++;
                        else if (r_old_center_x > r_center_x) x--;
                    }
                }
                
        } while (r_placed < room_num);  

        //dungeon stairs gen
        do
        {
            sy = rand() % rows;
            sx = rand() % cols;
        }
        while (map[sy][sx] != ' ');
        map[sy][sx] = '>'; 
        

        // dungeon shop
        if(dlvl == 1) // dlvl >= 5 && dlvl % 5 == 0
        {
            do
            {
                sy = rand() % rows;
                sx = rand() % cols;   
            }
                while (map[sy][sx] != ' ');
                map[sy][sx] = '$';
        }
    }

    return 0;
}

int game_loop(int c, int rows, int cols, char (* map) [cols])
{
    bool new_lvl = 0;
    static bool first_run = 1; 


    if (first_run || r_placed == 0 )
    {
        dungeon_gen(rows, cols, map);
        respawn_ent(rows, cols, map);
        first_run = 0;
    }
    

    if (c != 0)
    {
        new_lvl = p_action(c, cols, rows, map); // + battle()
    }

    monster_turn(rows, cols, map);
    dungeon_draw(rows, cols, map);

    attron(A_BOLD);
        mvaddch(py,px, '@'); // draw playa
    attroff(A_BOLD);
    
    if (show_casino_message) {
        mvprintw(0,0, " C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0 C4S1N0");
        show_casino_message = 0;
    }

    if (show_kill_message) {

        // attron(COLOR_PAIR(GREEN));
            mvprintw(0,0, "\t>> You've killed an enemie.");
        // attroff(COLOR_PAIR(GREEN));
        
        show_kill_message = 0;
    }

    if (show_damage_message) {

        // attron(COLOR_PAIR(RED));
            mvprintw(0,0, "\t>> You have recieved some damage.");
        // attroff(COLOR_PAIR(RED));
        
        show_damage_message = 0;
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
                hp = 10;
                p_gold = 0;
                dlvl = 1;
                s_placed = 0;
                p_placed = 0;
                r_placed = 0;
                s_defeated = 0;
                first_run = 0;
                dungeon_gen(rows, cols, map);
                respawn_ent(rows, cols, map);

                clear();
                mvprintw(rows /2, cols /2 - 25, "Welcome back to level %d. (Press any button to continue)", dlvl);
                break; // Продолжаем игровой цикл
                
            }
        }

    }

    refresh();

    c = getch();

    return c;
}

int main (void)
{
    int c = 0; // input
    int rows, cols;
    srand(time(NULL));

    initscr();

    start_color();
    use_default_colors();

    // init color pairs* 1st atr linked with color const //
    init_pair (RED, COLOR_RED, COLOR_BLACK);
    init_pair (GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair (YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair (BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair (MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair (CYAN, COLOR_CYAN, COLOR_BLACK);

    keypad(stdscr, 1); // allow arrows

    noecho(); // no input echo
    curs_set(0); // hide cursor

    getmaxyx(stdscr, rows, cols);
    printw("rows: %d, cols: %d", rows, cols);

    char map[rows][cols]; // 0 - 119

    do
    { 
        c = game_loop(c, rows - 1, cols, map); // rows - 1 cause 0 - 119

    } while (c != 27); // 27 ASCII = ESC


    refresh();
    endwin();

    return 0;
}