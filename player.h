#ifndef PLAYER_H
#define PLAYER_H

int respawn_player(int rows, int cols, char (* map) [cols]);
int p_action(int c, int cols, int rows, char (* map) [cols]);
int battle(int cols, char (* map) [cols], int dir_y, int dir_x);

#endif