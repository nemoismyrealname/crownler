#include <ncurses.h>
#include <stdlib.h>
#include "globals.h"
#include "dungeon.h"

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