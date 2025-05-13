#include <ncurses.h>
#include <stdlib.h>
#include "enemies.h"
#include "globals.h"

int respawn_enemies(int rows, int cols, char (* map) [cols])
{   
    if (!s_placed)
    {   
        int my, mx;
        int max_attempts = 1000;
        
        for(int m = 0; m < 10; m++)
        {   
            int attempts = 0;

            while (attempts < max_attempts)
            {
                my = rand() % rows;
                mx = rand() % cols;

                int dist_y = abs(my - py);
                int dist_x = abs(mx - px);
                
                if (map[my][mx] == ' ' && dist_y >= 3 && dist_x >= 3)
                {
                    break;
                }

                attempts++;
            }

            if (attempts >= max_attempts)
            {
                do
                {
                    my = rand() % rows;
                    mx = rand() % cols;
                } 
                while (map[my][mx] != ' ');
            }

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