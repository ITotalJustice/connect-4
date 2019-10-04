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
    // check for empty space from the bottom up.
    for (int y = HEIGHT; y >= 0; y--)
    {
        if (board[y][pos_x] == EMPTY) return y;
    }

    // if nothing found.
    return NO_EMPTY;
}

void finished_game_message(char *message)
{
    printf("\n\n%s", message);
    printf("\n\nPress (+) to exit...");
    consoleUpdate(NULL);

    while (1)
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_PLUS) break;
    }
}

int draw_state(char board[HEIGHT][WIDTH])
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (board[0][i] == EMPTY) break;

        // if its reached the end of the board.
        if (i == WIDTH - 1) return DRAW;
    }

    return CONTINUE;
}

int horizontal_win_state(char board[HEIGHT][WIDTH], char player)
{
    for (int i = HEIGHT-1; i >= 0; i--)
    {
        for (int j = 0; j < WIDTH - 3; j++)
        {
            for (int counter = 0; counter != 4; counter++)
            {
                if (board[i][counter+j] != player) break;
                if (counter == 3) return WIN;
            }
        }
    }

    return CONTINUE;
}

int vertical_win_state(char board[HEIGHT][WIDTH], char player)
{
    for (int i = WIDTH-1; i >= 0; i--)
    {
        for (int j = 0; j < HEIGHT-3; j++)
        {
            for (int counter = 0; counter != 4; counter++)
            {
                if (board[counter+j][i] != player) break;
                if (counter == 3) return WIN;
            }
        }
    }

    return CONTINUE;
}

int check_game_state(char board[HEIGHT][WIDTH])
{
    // check for draw
    if (draw_state(board) == DRAW)
    {
        finished_game_message("DRAW");
        return DRAW;
    }

    // check if blue won...
    if (vertical_win_state(board, BLUE) == WIN || horizontal_win_state(board, BLUE) == WIN)
    {
        finished_game_message("BLUE WON");
        return WIN;
    }

    // check if red won...
    if (vertical_win_state(board, RED) == WIN || horizontal_win_state(board, RED) == WIN)
    {
        finished_game_message("RED WON");
        return WIN;
    }

    // if neither state met, continue.
    return CONTINUE;
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

                // exit if state is not continue (win / draw).
                if (check_game_state(board_grid) != CONTINUE) break;
            }
        }

        if (kDown & KEY_PLUS)
        {
            //in_game menu
            break;
        }
    }
}