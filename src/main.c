/*
 * main.c
 *
 *  Created on: 2019年2月23日
 *      Author: Wind
 */


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <winbase.h>
#include <wincon.h>
#include <windef.h>
#include <winnt.h>
#include <winuser.h>
#include <assert.h>

#include "base.h"
#include "main.h"
#include "map.h"
#include "show.h"
#include "log.h"

int g_iMainContinue;

void main_Exit(void);

BOOL CALLBACK main_ConsoleCtrlHandler(DWORD ulEvent)
{
    LOG_WARN("Closed by console ctrl cvent(%lu).", ulEvent);

    switch(ulEvent)
    {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        {
            main_Exit();
            break;
        }

        default:
        {
            assert(0);
            break;
        }
    }

    return TRUE;
}

int main_Process(void)
{
    int iErrCode = ERROR_SUCCESS;
    int iChar;

    iChar = 0;
    while (kbhit())
    {
        iChar = getch();
    }
    if (iChar)
    {
        iErrCode |= MAP_Input(MI_KEY, &iChar);
    }

    iErrCode |= MAP_Input(MI_TIME, NULL);

    return iErrCode;
}

int main_Init(void)
{
    int iErrCode = ERROR_SUCCESS;
    int iRet;

    iErrCode |= LOG_Init();

    iErrCode |= SHOW_Init();
    iErrCode |= MAP_Init();

    iRet = SetConsoleCtrlHandler(main_ConsoleCtrlHandler, TRUE);
    if (0 == iRet)
    {
        LOG_ERROR("Failed to set console ctrl event handler. result=%d.", iRet);
        iErrCode |= ERROR_FAILED;
    }

    if (ERROR_SUCCESS != iErrCode)
    {
        LOG_ERROR("Failed to init Tetris. error=%d.", iErrCode);
    }

    g_iMainContinue = 1;

    return iErrCode;
}

void main_Exit(void)
{
    g_iMainContinue = 0;
    (void) SetConsoleCtrlHandler(main_ConsoleCtrlHandler, FALSE);

    MAP_Exit();
    SHOW_Exit();
    LOG_Exit();

    return;
}

int main(void)
{
    int iErrCode;

    iErrCode = main_Init();
    if (ERROR_SUCCESS != iErrCode)
    {
        main_Exit();
        return EXIT_SUCCESS;
    }

    while(g_iMainContinue)
    {
        Sleep(MAIN_REFRESH_INTERVAL);

        iErrCode = main_Process();
        if(ERROR_EXIT == iErrCode)
        {
            g_iMainContinue = 0;
            iErrCode  = ERROR_SUCCESS;
            LOG_ERROR("Exit by map.");
        }
        else if(ERROR_SUCCESS != iErrCode)
        {
            g_iMainContinue = 0;
            LOG_ERROR("Failed to refresh map, and exit. error=%d");
        }
    }

    main_Exit();

    return EXIT_SUCCESS;
}
