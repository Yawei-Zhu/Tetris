/*
 ============================================================================
 Name        : Tetris.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <winbase.h>
#include <wincon.h>
#include <windef.h>
#include <winnt.h>
#include <winuser.h>

#include "base.h"
#include "map.h"
#include "show.h"


int _main_Init(void)
{
    int iErrCode;

    iErrCode  = SHOW_Init();
    iErrCode |= MAP_Init();

    return iErrCode;
}

void _main_Exit(void)
{
    MAP_Exit();
    SHOW_Exit();

    return;
}

int main(void)
{
    int iErrCode;
    int iContinue;
    int iChar;

    iErrCode = _main_Init();
    if (ERROR_SUCCESS != iErrCode)
    {
        puts("main: init failed.");
        return EXIT_SUCCESS;
    }

    iContinue = 1;
    while(iContinue)
    {
        Sleep(MAP_REFRESH_INTERVAL);

        iChar = 0;
        while (kbhit())
        {
            iChar = getch();
        }

        if (iChar)
        {
            MAP_Input(MI_KEY, &iChar);
        }

        iErrCode = MAP_Input(MI_TIME, NULL);
        if(ERROR_EXIT == iErrCode)
        {
            iContinue = 0;
            SHOW_ShowTip("map refresh exit.");
            iErrCode  = ERROR_SUCCESS;
        }
        else if(ERROR_SUCCESS != iErrCode)
        {
            SHOW_ShowTip("map refresh failed.");
            iContinue = 0;
        }
    }

    _main_Exit();

    return EXIT_SUCCESS;
}
