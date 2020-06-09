/*
 * show.c
 *
 *  Created on: 2018-09-02
 *      Author: Wind
 */


#include <stdio.h>
#include <stdlib.h>
#include <winbase.h>
#include <winnt.h>
#include <wincon.h>

#include "base.h"
#include "main.h"
#include "map.h"
#include "data.h"
#include "show.h"

HANDLE g_hShowConsoleOut = NULL;
int g_aaiShowCanvas[SHOW_ROW][SHOW_COL];
int g_iShowCursorVisible = 0;

int show_InitCanvas(void)
{
    system("cls");

    for(int y = 0; y < SHOW_ROW; y++)
    {
        for(int x = 0; x < SHOW_COL; x++)
        {
            g_aaiShowCanvas[y][x] = ' ';
            printf("%c", g_aaiShowCanvas[y][x]);
        }
        printf("\n");
    }

    return ERROR_SUCCESS;
}

void show_GotoXY(ushort x, ushort y)
{
    COORD pos;

    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(g_hShowConsoleOut, pos);

    return;
}

void _show_ShowChar(int c, ushort x, ushort y)
{
    show_GotoXY(x, y);

    printf("%c", c);

    return;
}

int show_ShowCanvas(int aaiCanvas[SHOW_ROW][SHOW_COL])
{
    CONSOLE_SCREEN_BUFFER_INFO stInfo;

    GetConsoleScreenBufferInfo(g_hShowConsoleOut, &stInfo);

    for(int y = 0; y < SHOW_ROW; y++)
    {
        for(int x = 0; x < SHOW_COL; x++)
        {
            if (g_aaiShowCanvas[y][x] == aaiCanvas[y][x])
            {
                continue;
            }

            _show_ShowChar(aaiCanvas[y][x], x, y);
        }
    }

    SetConsoleCursorPosition(g_hShowConsoleOut, stInfo.dwCursorPosition);

    return ERROR_SUCCESS;
}
int SHOW_ShowMap(int aaiMap[MAP_ROW][MAP_COL])
{
    int aaiCanvas[SHOW_ROW][SHOW_COL];

    memcpy(aaiCanvas, g_aaiShowCanvas, sizeof(aaiCanvas));

    for(int y = 0; y < MAP_ROW && y < SHOW_ROW; y++)
    {
        for(int x = 0; x < MAP_COL && x < SHOW_COL; x++)
        {
            aaiCanvas[y][x] = aaiMap[y][x];
        }
    }

    show_ShowCanvas(aaiCanvas);

    memcpy(g_aaiShowCanvas, aaiCanvas, sizeof(g_aaiShowCanvas));

    return ERROR_SUCCESS;
}

int SHOW_ShowNext(int aaiNext[4][4])
{
    int aaiCanvas[SHOW_ROW][SHOW_COL];

    memcpy(aaiCanvas, g_aaiShowCanvas, sizeof(aaiCanvas));

    for(int y = 0; y < 4 && y + 2 < SHOW_ROW; y++)
    {
        for(int x = 0; x < 4 && x + MAP_COL + 2 < SHOW_COL; x++)
        {
            aaiCanvas[y + 2][x + MAP_COL + 2] = aaiNext[y][x];
        }
    }

    show_ShowCanvas(aaiCanvas);

    memcpy(g_aaiShowCanvas, aaiCanvas, sizeof(g_aaiShowCanvas));

    return ERROR_SUCCESS;
}

int show_SetCursorVisible(int iVisible)
{
    int iOldVisible;
    CONSOLE_CURSOR_INFO stCursorInfo;

    GetConsoleCursorInfo(g_hShowConsoleOut, &stCursorInfo);
    iOldVisible = (int) stCursorInfo.bVisible;
    stCursorInfo.bVisible = (BOOL) iVisible;
    SetConsoleCursorInfo(g_hShowConsoleOut, &stCursorInfo);

    return iOldVisible;
}

int SHOW_Init()
{
    int iErrCode;

    g_hShowConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (NULL == g_hShowConsoleOut)
    {
        return ERROR_FAILED;
    }

    g_iShowCursorVisible = show_SetCursorVisible(0);

    iErrCode = show_InitCanvas();

    return iErrCode;
}

void SHOW_Exit()
{
    show_GotoXY(0, SHOW_ROW);
    show_SetCursorVisible(g_iShowCursorVisible);

    g_hShowConsoleOut = NULL;
}
