#ifndef _GAME_H_
#define _GAME_H_

#define HEIGHT      6
#define WIDTH       7

#define CONTINUE    0
#define WIN         1
#define LOSE        2
#define DRAW        3

void create_new_board(char board[HEIGHT][WIDTH]);
void draw_board(char board[HEIGHT][WIDTH]);
void add_counter_to_board(char board[HEIGHT][WIDTH], char player, int pos_y, int pos_x);
int is_space_empty(char board[HEIGHT][WIDTH], int pos_x);
void start_game();

#endif