#include <stdio.h>
#include <switch.h>

#include "sdl.h"
#include "game.h"
#include "hid.h"

#define MAIN_MENU_LIST_MAX      3
#define OPTIONS_MENU_LIST_MAX   5

#define MENU_START              0
#define MENU_OPTIONS            1
#define MENU_QUIT               2

#define OPTIONS_P1_COLOUR       0
#define OPTIONS_P2_COLOUR       1
#define OPTIONS_UPDATE          2
#define OPTIONS_SAVE_EXIT       3
#define OPTIONS_EXIT            4


void print_option_list(char **options, int cursor, int list_max)
{
    SDL_ClearRenderer();

    for (int i = 0, nl = 25; i < list_max; i++, nl+=50)
    {
        if (i == cursor)
            SDL_DrawText(fntSmall, 25, nl, grey, options[i]);
        else
            SDL_DrawText(fntSmall, 25, nl, black, options[i]);
    }

    SDL_UpdateRenderer();
}

void options_menu()
{
    int cursor = 0;
    char *options[] = {"Player 1 Colour", "Player 2 Colour", "Update Game", "Save & Quit", "Quit"};

    print_option_list(options, cursor, OPTIONS_MENU_LIST_MAX);

    while (1)
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, OPTIONS_MENU_LIST_MAX - 1);
            print_option_list(options, cursor, OPTIONS_MENU_LIST_MAX);
        }

        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, OPTIONS_MENU_LIST_MAX - 1);
            print_option_list(options, cursor, OPTIONS_MENU_LIST_MAX);
        }

        if (kDown & KEY_A)
        {
            switch (cursor)
            {
                case OPTIONS_P1_COLOUR:
                //player 1 colour
                break;

                case OPTIONS_P2_COLOUR:
                //player 2 colour
                break;

                case OPTIONS_UPDATE:
                //update game
                break;

                case OPTIONS_SAVE_EXIT:
                //save + exit
                return;

                case OPTIONS_EXIT:
                //exit
                return;
            }
        }
    }
}

void main_menu()
{
    int cursor = 0;
    char *options[] = {"Start Game", "Options", "Exit Game"};

    print_option_list(options, cursor, MAIN_MENU_LIST_MAX);

    while (1)
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, MAIN_MENU_LIST_MAX - 1);
            print_option_list(options, cursor, MAIN_MENU_LIST_MAX);
        }

        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, MAIN_MENU_LIST_MAX - 1);
            print_option_list(options, cursor, MAIN_MENU_LIST_MAX);
        }

        if (kDown & KEY_A)
        {
            switch (cursor)
            {
                case MENU_START:
                //1 vs 1
                //1 vs cpu
                start_game(P_V_P);
                break;

                case MENU_OPTIONS:
                options_menu();
                break;

                case MENU_QUIT:
                // exit.
                return;
            }
            print_option_list(options, cursor, MAIN_MENU_LIST_MAX);
        }
    }
}