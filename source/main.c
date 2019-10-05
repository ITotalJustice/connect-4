#include <stdio.h>
#include <switch.h>

#include "menu.h"
#include "sdl.h"


void appInit()
{
    #ifdef DEBUG
    socketInitializeDefault();
    nxlinkStdio();
    #endif
    plInitialize();
    sdlInit();
    //consoleInit(NULL);
}

void appExit()
{
    #ifdef DEBUG
    socketExit();
    #endif
    plExit();
    sdlExit();
    //consoleExit(NULL);
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