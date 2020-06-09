/*
 * main.c
 *
 *  Created on: 2019-02-23
 *      Author: Wind
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <windows.h>
#include <conio.h>
#include <pthread.h>

#include "base.h"
#include "list.h"
#include "main.h"
#include "map.h"
#include "data.h"
#include "show.h"
#include "log.h"
#include "queue.h"

int g_iMainContinue = 1;

QUE_HANDLE g_hMainQue = QUE_INVALID_HANDLE;
uint  g_uiMainTimer = 0;

void main_Exit(void);

BOOL CALLBACK main_ConsoleCtrlHandler(DWORD ulEvent)
{
    LOG_WARN("Closed by console ctrl event(%lu).", ulEvent);

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

void CALLBACK main_TimeEventCallback(uint uiTimeId, uint uiMsg, ulong ulUser,
        ulong ul1, ulong ul2)
{
    int iRet;

    LOG_DEBUG("Timer timeout.");
    iRet = QUE_Write(g_hMainQue, (void *) 1);
    return;
}

int main_Process(void)
{
    int iErrCode = ERROR_SUCCESS;
    int iChar;
    int iMsg;

    iMsg = (int) QUE_Read(g_hMainQue);

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

int main_ProcessAll(void)
{
    int iErrCode;

    while(g_iMainContinue)
    {
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

    return iErrCode;
}

int main_Init(void)
{
    int iErrCode = ERROR_SUCCESS;
    int iRet;

    g_hMainQue = QUE_Init(NULL);
    if (QUE_INVALID_HANDLE == g_hMainQue)
    {
        return ERROR_FAILED;
    }

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
    g_uiMainTimer = timeSetEvent(MAIN_REFRESH_INTERVAL, 1, main_TimeEventCallback, 0, TIME_PERIODIC);

    return iErrCode;
}

void main_Exit(void)
{
    g_iMainContinue = 0;
    (void) SetConsoleCtrlHandler(main_ConsoleCtrlHandler, FALSE);

    MAP_Exit();
    SHOW_Exit();
    LOG_Exit();

    QUE_Exit(g_hMainQue);
    g_hMainQue = QUE_INVALID_HANDLE;

    return;
}

int main(void)
{
    int iErrCode;

    iErrCode = main_Init();

    if (ERROR_SUCCESS == iErrCode)
    {
        iErrCode = main_ProcessAll();
    }

    main_Exit();

    return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
