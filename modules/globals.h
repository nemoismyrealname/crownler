//h guard
#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

// color consts
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6

// monsters struct
struct monsters
{
    int y;
    int x;
    int lvl;
    int type;
    bool awake;

    bool elite;
    bool boss;

    bool red;
    bool yellow;
    bool white;
};

// global consts
extern int sy, sx;
extern int py, px; 
extern int hp;
extern int att;
extern int p_gold;
extern int xp;
extern int p_lvl;
extern int dlvl;
extern int xp_to_next_level;

extern int armor;
extern int MAX_ARMOR;

extern bool s_placed;
extern bool p_placed;
extern int r_placed;

extern int s_defeated;
extern bool show_casino_message;
extern bool show_shop_message;
extern bool show_kill_message;
extern bool show_damage_message;
extern bool show_miss_message;
extern bool show_heal_message;
extern bool show_attack_message;

extern int session;

extern struct monsters monster[10];

#endif