#include <stdio.h>
#include <switch.h>

#include "menu.h"


void appInit()
{
    #ifdef DEBUG
    socketInitializeDefault();
    nxlinkStdio();
    #endif
    consoleInit(NULL);
}

void appExit()
{
    #ifdef DEBUG
    socketExit();
    #endif
    consoleExit(NULL);
}

int main(int arc, char **argv)
{
    // init everything for the app.
    appInit();

    // start main menu.
    main_menu();

    // exit app if returned here.
    appExit();
    return 0;
}