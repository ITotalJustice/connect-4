#include <stdio.h>
#include <switch.h>

#include "game.h"
#include "menu.h"
#include "sdl.h"
#include "hid.h"

#define YELLOW          'X'
#define RED             'O'
#define EMPTY           ' '

#define CONTINUE        0
#define WIN             1
#define TIE             2

#define PLAYER_1        0
#define PLAYER_2        1

#define RIGHT           0
#define LEFT            1

#define NO_EMPTY        10

#define COUNTER_SIZE    50  //sqaure for now
#define BOARD_X         270
#define BOARD_Y         120
#define VERTICAL_NL     75
#define HORIZONTAL_NL   100


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
    SDL_ClearRenderer();

    SDL_DrawShape(grey, 20, 22, 200, 40);
    SDL_DrawText(fntSmall, 25, 25, yellow, "PLAYER 1");
    SDL_DrawShape(grey, 20, 72, 200, 40);
    SDL_DrawText(fntSmall, 25, 75, red, "PLAYER 2");

    // the blue board.
    SDL_DrawShape(blue, BOARD_X, BOARD_Y, (100*WIDTH)+20, (75*HEIGHT)+20);

    for (int i = 0, hl = BOARD_Y+20; i < HEIGHT; i++, hl+=VERTICAL_NL)
    {
        for (int j = 0, wl = BOARD_X+30; j < WIDTH; j++, wl+=HORIZONTAL_NL)
        {
            if (board[i][j] == EMPTY)
            SDL_DrawShape(white, wl, hl, COUNTER_SIZE, COUNTER_SIZE);

            else if (board[i][j] == YELLOW)
            SDL_DrawShape(yellow, wl, hl, COUNTER_SIZE, COUNTER_SIZE);

            else
            SDL_DrawShape(red, wl, hl, COUNTER_SIZE, COUNTER_SIZE);
        }
    }
}

void show_cursor_position(char board[HEIGHT][WIDTH], char player, int pos_y, int pos_x)
{
    // add the counter to the board.
    board[pos_y][pos_x] = player;
    draw_board(board);
    SDL_UpdateRenderer();
    // remove it after displaying the board.
    board[pos_y][pos_x] = EMPTY;
}

void add_counter_to_board(char board[HEIGHT][WIDTH], char player, int pos_y, int pos_x)
{
    board[pos_y][pos_x] = player;
    draw_board(board);
    SDL_UpdateRenderer();
}

int is_space_empty(char board[HEIGHT][WIDTH], int pos_x)
{
    // check for empty space from the bottom up.
    for (int y = HEIGHT; y >= 0; y--)
    {
        if (board[y][pos_x] == EMPTY) return y;
    }

    // if no empty space found.
    return NO_EMPTY;
}

void finished_game_message(char *message)
{
    SDL_DrawText(fntSmall, 25, 625, black, message);
    SDL_DrawText(fntSmall, 25, 675, black, "Press (+) to exit...");

    SDL_UpdateRenderer();

    while (1)
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_PLUS) break;
    }
}

int tie_state(char board[HEIGHT][WIDTH])
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (board[0][i] == EMPTY) break;

        // if its reached the end of the board.
        if (i == WIDTH - 1) return TIE;
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

int left_diagonal_win_state(char board[HEIGHT][WIDTH], char player)
{
    for (int i = HEIGHT-1; i >= 0; i--)
    {
        for (int j = 0; j < WIDTH-3; j++)
        {
            for (int counter = 0; counter != 4; counter++)
            {
                if (board[i-counter][counter+j] != player) break;
                if (counter == 3) return WIN;
            }
        }
    }

    return CONTINUE;
}

int right_diagonal_win_state(char board[HEIGHT][WIDTH], char player)
{
    for (int i = HEIGHT-1; i >= 0; i--)
    {
        for (int j = WIDTH-1; j > 2; j--)
        {
            for (int counter = 0; counter != 4; counter++)
            {
                if (board[i-counter][j-counter] != player) break;
                if (counter == 3) return WIN;
            }
        }
    }
    
    return CONTINUE;
}

int check_game_state(char board[HEIGHT][WIDTH])
{
    // check for draw
    if (tie_state(board) == TIE)
    {
        draw_board(board);
        finished_game_message("DRAW");
        return TIE;
    }

    // check if blue won...
    if (vertical_win_state(board, YELLOW) == WIN || horizontal_win_state(board, YELLOW) == WIN || left_diagonal_win_state(board, YELLOW) == WIN || right_diagonal_win_state(board, YELLOW) == WIN)
    {
        draw_board(board);
        finished_game_message("YELLOW WON");
        return WIN;
    }

    // check if red won...
    if (vertical_win_state(board, RED) == WIN || horizontal_win_state(board, RED) == WIN || left_diagonal_win_state(board, RED) == WIN || right_diagonal_win_state(board, RED) == WIN)
    {
        draw_board(board);
        finished_game_message("RED WON");
        return WIN;
    }

    // if neither state met, continue.
    return CONTINUE;
}

void start_game(int game_mode)
{
    // create array of counters. Set turn to PLAYER_1.
    char counter_array[] = {YELLOW, RED};
    int turn = PLAYER_1;

    // set cursor position.
    int cursor_pos_y = HEIGHT - 1;
    int cursor_pos_x = 0;

    // create the board array.
    char board_grid[HEIGHT][WIDTH];

    // create new empty board.
    create_new_board(board_grid);

    // draw that empty board.
    show_cursor_position(board_grid, counter_array[turn], cursor_pos_y, cursor_pos_x);

    // exit if state is not continue (win / draw).
    while (check_game_state(board_grid) == CONTINUE)
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

            show_cursor_position(board_grid, counter_array[turn], cursor_pos_y, cursor_pos_x);
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

            show_cursor_position(board_grid, counter_array[turn], cursor_pos_y, cursor_pos_x);
        }

        if (kDown & KEY_A)
        {
            cursor_pos_y = is_space_empty(board_grid, cursor_pos_x);

            // if the current_pos is empty, add a counter.
            if (cursor_pos_y != NO_EMPTY)
            {
                add_counter_to_board(board_grid, counter_array[turn], cursor_pos_y, cursor_pos_x);
                if (turn == PLAYER_1) turn = PLAYER_2;
                else turn = PLAYER_1;
            }
        }

        if (kDown & KEY_PLUS)
        {
            //game_options_menu(board_grid);
            break;
        }
    }
}