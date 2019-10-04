#include <stdio.h>
#include <switch.h>

#include "game.h"
#include "menu.h"
#include "hid.h"

#define BLUE        'O'
#define RED         'X'
#define EMPTY       ' '

#define PLAYER_1    0
#define PLAYER_2    1

#define RIGHT       0
#define LEFT        1

#define NO_EMPTY    10


void create_new_board(char board[HEIGHT][WIDTH])
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            board[i][j] = EMPTY;
        }
    }
}

void draw_board(char board[HEIGHT][WIDTH])
{
    consoleClear();

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("|%c", board[i][j]);
        }
        printf("|\n");
        for (int j = 0; j < WIDTH; j++)
        {
            printf("--");
        }
        printf("-\n");
    }

    consoleUpdate(NULL);
}

void show_cursor_position(char board[HEIGHT][WIDTH], char player, int pos_y, int pos_x)
{
    // add the counter to the board.
    board[pos_y][pos_x] = player;
    draw_board(board);
    // remove it after displaying the board.
    board[pos_y][pos_x] = EMPTY;
}

void add_counter_to_board(char board[HEIGHT][WIDTH], char player, int pos_y, int pos_x)
{
    board[pos_y][pos_x] = player;
    draw_board(board);
}

int is_space_empty(char board[HEIGHT][WIDTH], int pos_x)
{
    int y = HEIGHT-1;

    // check for empty space from the bottom up.
    while (y >= 0)
    {
        if (board[y][pos_x] == EMPTY) return y;
        y--;
    }

    // if nothing found.
    return NO_EMPTY;
}

void start_game()
{
    // create the board array.
    char board_grid[HEIGHT][WIDTH];

    // create new empty board.
    create_new_board(board_grid);

    // draw that empty board.
    draw_board(board_grid);

    // create array of players. Set turn to PLAYER_1.
    char player_array[] = {BLUE, RED};
    int turn = PLAYER_1;

    // set cursor position.
    int cursor_pos_y = HEIGHT - 1;
    int cursor_pos_x = 0;

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_LEFT)
        {
            cursor_pos_x = move_cursor_left(cursor_pos_x, WIDTH-1);
            cursor_pos_y = is_space_empty(board_grid, cursor_pos_x);

            while (cursor_pos_y == NO_EMPTY)
            {
                cursor_pos_x = move_cursor_left(cursor_pos_x, WIDTH-1);
                cursor_pos_y = is_space_empty(board_grid, cursor_pos_x);
            }

            show_cursor_position(board_grid, player_array[turn], cursor_pos_y, cursor_pos_x);
        }

        if (kDown & KEY_RIGHT)
        {
            cursor_pos_x = move_cursor_right(cursor_pos_x, WIDTH-1);
            cursor_pos_y = is_space_empty(board_grid, cursor_pos_x);

            while (cursor_pos_y == NO_EMPTY)
            {
                cursor_pos_x = move_cursor_right(cursor_pos_x, WIDTH-1);
                cursor_pos_y = is_space_empty(board_grid, cursor_pos_x);
            }

            show_cursor_position(board_grid, player_array[turn], cursor_pos_y, cursor_pos_x);
        }

        if (kDown & KEY_A)
        {
            cursor_pos_y = is_space_empty(board_grid, cursor_pos_x);

            // if the current_pos is empty, add a counter.
            if (cursor_pos_y != NO_EMPTY)
            {
                add_counter_to_board(board_grid, player_array[turn], cursor_pos_y, cursor_pos_x);
                if (turn == PLAYER_1) turn = PLAYER_2;
                else turn = PLAYER_1;
            }
        }

        if (kDown & KEY_PLUS)
        {
            //in_game menu
            break;
        }
    }
}