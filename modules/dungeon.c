#include <ncurses.h>
#include <stdlib.h>
#include "globals.h"
#include "dungeon.h"

int dungeon_gen(int rows, int cols, char (* map)[cols])
{
    if (!r_placed)
    {
        int ry, rx; // room coords
        int r_size_y, r_size_x; // room size
        int r_center_y, r_center_x;
        int r_old_center_y, r_old_center_x;
        int room_num = rand() % 5 + 5;
        bool collision;

        // fill dungeon with walls and borders
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {          
                // borders
                if (y == 0 || y == 1 || y == rows - 1 || x == 0 ||
                    x == cols - 1 || y == rows)
                    map[y][x] = '%';
                // walls
                else            
                    map[y][x] = '#';
            }
        }

        while (r_placed < room_num)
        {
            int try_counter = 0; // number of tries for prototyping
            
            // prototyping the room until no collisions
            do
            {
                collision = 0;
                
                // room coords
                ry = rand() % (rows - 4) + 1;
                rx = rand() % (cols - 4) + 1;
                
                // room sizes
                r_size_y = rand() % 5 + 4;
                r_size_x = rand() % 10 + 8;
                
                try_counter++;
                if (try_counter > 100)
                {
                    ry = rx = 3;
                    r_size_y = r_size_x = 3;
                    break;
                }
                
                // check for collision
                for (int y = ry; y <= ry + r_size_y; y++)
                {
                    for (int x = rx; x <= rx + r_size_x; x++)
                    {
                        if (map[y][x] == '%' || map[y][x] == ' ' ||
                            map[y + 2][x] == ' ' || map[y - 2][x] == ' ' ||
                            map[y][x + 2] == ' ' || map[y][x - 2] == ' ')
                        {
                            collision = 1;
                            y = ry + r_size_y + 1; // exit upper loop..
                            break; // ..exit from current loop
                        }
                    }
                }
            }
            while (collision == 1);
            
            // fill DB map with rooms
            for (int y = ry; y <= ry + r_size_y; y++)
            {
                for (int x = rx; x <= rx + r_size_x; x++)
                {
                    if (map[y][x] == '%')
                    {
                        y = ry + r_size_y + 1; // exit upper loop..
                        break; // ..exit from current loop
                    }
                    else
                        map[y][x] = ' ';
                }
            }
            
            r_placed++;
            
            // corridors
            if (r_placed > 1)
            {
                r_old_center_y = r_center_y;
                r_old_center_x = r_center_x;
            }

            r_center_y = ry + (r_size_y / 2);
            r_center_x = rx + (r_size_x / 2);
            
            if (r_placed > 1)
            {
                int path_y;

                for (path_y = r_old_center_y; path_y != r_center_y; )
                {
                    if (map[path_y][r_old_center_x] != '%')
                        map[path_y][r_old_center_x] = ' ';
                    
                    if (r_old_center_y < r_center_y)
                        path_y++;
                    else if (r_old_center_y > r_center_y)
                        path_y--;
                }
                
                for (int path_x = r_old_center_x; path_x != r_center_x; )
                {
                    if (map[path_y][path_x] != '%')
                        map[path_y][path_x] = ' ';
                    
                    if (r_old_center_x < r_center_x)
                        path_x++;
                    else if (r_old_center_x > r_center_x)
                        path_x--;
                }
            }
        }

        // stairs gen with min distance from p
        int stair_attempts = 0;
        const int min_distance = 10;
        int dist_y, dist_x;

        do
        {
            sy = rand() % rows;
            sx = rand() % cols;
            dist_y = abs(sy - py);
            dist_x = abs(sx - px);
            stair_attempts++;
            if (stair_attempts > 2000)
            {
                mvprintw(0, 0, "Failed to place stairs on level %d after %d attempts", dlvl, stair_attempts);
                refresh();
                break;
            }
        }
        while (sy >= rows - 1 || sx >= cols - 1 || map[sy][sx] != ' ' || (dist_y < min_distance && dist_x < min_distance));
        
        map[sy][sx] = '>';
        

        // dungeon shop
        if(dlvl >= 3 && dlvl % 3 == 0) 
        {   
            int shop_dist_y, shop_dist_x;
            int shop_y, shop_x;
            do
            {
                shop_y = rand() % rows;
                shop_x = rand() % cols;   

                shop_dist_y = abs(shop_y - py);
                shop_dist_x = abs(shop_x - px);
            }
                while (shop_y >= rows || shop_x >= cols || map[shop_y][shop_x] != ' ' || map[shop_y][shop_x] == '>' || (shop_dist_y >= 10 && shop_dist_x >= 10));
                map[shop_y][shop_x] = '$';
        }
    }
    
    return 0;
}