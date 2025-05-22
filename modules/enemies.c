#include <ncurses.h>
#include <stdlib.h>
#include "enemies.h"
#include "globals.h"
#include "faq.h" // faq collector

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

                if (my >= rows || mx >= cols) continue;

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
                while (my >= rows || mx >= cols || map[my][mx] != ' ');
                mvprintw(0, 0, "Fallback spawn for monster %d on level %d", m, dlvl); // debug
                refresh();
            }

            monster[m].y = my;
            monster[m].x = mx;

            monster[m].lvl = rand() % dlvl + dlvl;

            //boss
            if (dlvl % 10 == 0 && dlvl >= 10 && m == 0)
            { 
                monster[m].boss = true;
                monster[m].lvl *= 2;
            } 

            // elite
            if (rand() % 7 == 0 && !monster[m].boss && dlvl >= 5) // 20% chance 
            {   
                monster[m].elite = true;
                monster[m].lvl = (monster[m].lvl * 3) / 2; // +50% HP
            }
            else if (dlvl < 5 && (rand() % 10 == 0) && !monster[m].elite && !monster[m].boss)
            {
                monster[m].lvl = dlvl / 2 + 1;
                monster[m].red = 1;
            }

            //yellow
            else if (dlvl < 10 && (rand() % 8 == 0) && !monster[m].elite && !monster[m].boss)
            {   
                monster[m].lvl = dlvl + 1;
                monster[m].type += 1;
                monster[m].yellow = 1;
            }
            
            //white
            if (rand() % 2 && !monster[m].elite && !monster[m].boss && !monster[m].red && !monster[m].yellow)
            {
                monster[m].lvl = dlvl + 2;
                monster[m].white = 1;
            }

            monster[m].type = rand() % (dlvl > 0 ? dlvl : 1) + 97; //97(a) - 122(z)

            
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

            //attack
            if (dist_y < 2 && dist_x < 2)
            {   
                //miss
                if (rand() % 5 == 0) 
                {
                    show_miss_message = 1;
                    continue;
                }
                
                int damage = (dlvl / 2) + (p_lvl / 2) + 1;
                if (monster[m].elite)
                damage = (damage * 3) / 2;

                if (monster[m].boss)
                damage *= 2;

                //armor
                int damage_including_armor = 100 - (armor * 20); // estimating damage %
                if (damage_including_armor < 0) damage_including_armor = 0; // !> 100%
                damage = (damage * damage_including_armor) / 100;

                hp -= damage; 
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

FAQEntry* get_enemy_faq()
{
    static FAQEntry entry;
    static char buffer[150];
    int base_damage = (dlvl / 2) + (p_lvl / 2) + 1;
    int elite_damage = (base_damage * 3) / 2;
    int boss_damage = base_damage * 2;

    int min_hp = dlvl;
    int max_hp = 2 * dlvl - 1;
    int min_hp_elite = (min_hp * 3) / 2;
    int max_hp_elite = (max_hp * 3) / 2;
    int min_hp_boss = min_hp * 2;
    int max_hp_boss = max_hp * 2;

    if (dlvl < 5) {
        snprintf(buffer, sizeof(buffer), "Regular: Dmg %d, HP %d-%d", 
                 base_damage, min_hp, max_hp);
    }
    else if (dlvl % 5 == 0 && dlvl >= 5) {
        snprintf(buffer, sizeof(buffer), "Regular: Dmg %d, HP %d-%d\n\tElite: Dmg %d, HP %d-%d", 
                 base_damage, min_hp, max_hp, elite_damage, min_hp_elite, max_hp_elite);
    }
    else if (dlvl % 10 == 0 && dlvl >= 10) {
        snprintf(buffer, sizeof(buffer), "Regular: Dmg %d, HP %d-%d\n\tElite: Dmg %d, HP %d-%d\n\tBoss: Dmg %d, HP %d-%d", 
                 base_damage, min_hp, max_hp, elite_damage, min_hp_elite, max_hp_elite, 
                 boss_damage, min_hp_boss, max_hp_boss);
    }

    entry.label = "Enemy status";
    entry.value = buffer;

    return &entry;
}

void init_enemies_faq()
{
    if (faq_data_count < 10) // 10 = max faq data sources
    { 
        get_faq_data[faq_data_count++] = get_enemy_faq;
    }
}

__attribute__((constructor))
void enemies_init()
{
    init_enemies_faq();
}
