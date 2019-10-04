#include <stdio.h>
#include <switch.h>

#include "menu.h"
#include "game.h"
#include "hid.h"

#define MENU_LIST_MAX   3
#define OPTION_LIST_MAX 5

#define START           0
#define OPTIONS         1
#define QUIT            2


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
                case 0:
                //player 1 colour
                break;

                case OPTIONS:
                //player 2 colour
                break;

                case 2:
                //update game
                break;

                case 3:
                //save + exit
                return;

                case 4:
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
                case START:
                //new game
                //load game
                start_game();
                break;

                case OPTIONS:
                options_menu();
                break;

                case QUIT:
                // exit.
                return;
            }
            print_option_list(options, cursor, MENU_LIST_MAX);
        }
    }
}