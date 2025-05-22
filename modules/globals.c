#include "globals.h"

//global consts
int sy, sx;
int py, px; 
int hp = 15;
int att = 1;
int xp = 0;
int p_gold = 0;
int p_lvl = 1;
int dlvl = 1;
int xp_to_next_level = 100;

int armor = 0;
int MAX_ARMOR = 5;

bool s_placed = 0;
bool p_placed = 0;
int r_placed = 0;

int s_defeated = 0;
bool show_casino_message = 0;
bool show_shop_message = 0;
bool show_kill_message = 0;
bool show_damage_message = 0;
bool show_miss_message = 0;
bool show_heal_message = 0;
bool show_attack_message = 0;

int session = 0;

struct monsters monster[10];
