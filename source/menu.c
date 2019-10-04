#include <stdio.h>
#include <switch.h>

#include "game.h"
#include "hid.h"

#define MENU_LIST_MAX   3
#define OPTION_LIST_MAX 5

#define MENU_START          0
#define MENU_OPTIONS        1
#define MENU_QUIT           2

#define OPTIONS_P1_COLOUR   0
#define OPTIONS_P2_COLOUR   1
#define OPTIONS_UPDATE      2
#define OPTIONS_SAVE_EXIT   3
#define OPTIONS_EXIT        4


void print_option_list(char **options, int cursor, int list_max)
{
    consoleClear();

    for (int i = 0; i < list_max; i++)
    {
        if (i == cursor) printf("> %s\n\n", options[i]);
        else printf("%s\n\n", options[i]);
    }

    consoleUpdate(NULL);
}

void options_menu()
{
    int cursor = 0;
    char *options[] = {"Player 1 Colour:", "Player 2 Colour", "Update Game", "Save", "Quit"};

    print_option_list(options, cursor, OPTION_LIST_MAX);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, OPTION_LIST_MAX - 1);
            print_option_list(options, cursor, OPTION_LIST_MAX);
        }

        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, OPTION_LIST_MAX - 1);
            print_option_list(options, cursor, OPTION_LIST_MAX);
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

    print_option_list(options, cursor, MENU_LIST_MAX);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, MENU_LIST_MAX - 1);
            print_option_list(options, cursor, MENU_LIST_MAX);
        }

        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, MENU_LIST_MAX - 1);
            print_option_list(options, cursor, MENU_LIST_MAX);
        }

        if (kDown & KEY_A)
        {
            switch (cursor)
            {
                case MENU_START:
                //new game
                //load game
                start_game();
                break;

                case MENU_OPTIONS:
                options_menu();
                break;

                case MENU_QUIT:
                // exit.
                return;
            }
            print_option_list(options, cursor, MENU_LIST_MAX);
        }
    }
}