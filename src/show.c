/*
 * show.c
 *
 *  Created on: 2018年9月2日
 *      Author: Wind
 */


#include <stdio.h>
#include <stdlib.h>
#include <winbase.h>
#include <winnt.h>
#include <wincon.h>

#include "base.h"
#include "map.h"
#include "show.h"

HANDLE g_hConsoleOut = NULL;
int g_aaiCanvas[SHOW_ROW][SHOW_COL];

int _show_InitCanvas(void)
{
    system("cls");

    for(int y = 0; y < SHOW_ROW; y++)
    {
        for(int x = 0; x < SHOW_COL; x++)
        {
            g_aaiCanvas[y][x] = ' ';
            printf("%c", g_aaiCanvas[y][x]);
        }
        printf("\n");
    }

    return ERROR_SUCCESS;
}

void _show_GotoXY(ushort x, ushort y)
{
    COORD pos;

    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(g_hConsoleOut, pos);

    return;
}

void _show_ShowChar(int c, ushort x, ushort y)
{
    _show_GotoXY(x, y);

    printf("%c", c);

    return;
}

int _show_ShowCanvas(int aaiCanvas[SHOW_ROW][SHOW_COL])
{
    for(int y = 0; y < SHOW_ROW; y++)
    {
        for(int x = 0; x < SHOW_COL; x++)
        {
            if (g_aaiCanvas[y][x] == aaiCanvas[y][x])
            {
                continue;
            }

            _show_ShowChar(aaiCanvas[y][x], x, y);
        }
    }

    return ERROR_SUCCESS;
}
int SHOW_ShowMap(int aaiMap[MAP_ROW][MAP_COL])
{
    int aaiCanvas[SHOW_ROW][SHOW_COL];

    memcpy(aaiCanvas, g_aaiCanvas, sizeof(aaiCanvas));

    for(int y = 0; y < MAP_ROW && y < SHOW_ROW; y++)
    {
        for(int x = 0; x < MAP_COL && x < SHOW_COL; x++)
        {
            aaiCanvas[y][x] = aaiMap[y][x];
        }
    }

    _show_ShowCanvas(aaiCanvas);

    memcpy(g_aaiCanvas, aaiCanvas, sizeof(g_aaiCanvas));

    return ERROR_SUCCESS;
}

int SHOW_ShowNext(int aaiNext[4][4])
{
    int aaiCanvas[SHOW_ROW][SHOW_COL];

    memcpy(aaiCanvas, g_aaiCanvas, sizeof(aaiCanvas));

    for(int y = 0; y < 4 && y + 2 < SHOW_ROW; y++)
    {
        for(int x = 0; x < 4 && x + MAP_COL + 2 < SHOW_COL; x++)
        {
            aaiCanvas[y + 2][x + MAP_COL + 2] = aaiNext[y][x];
        }
    }

    _show_ShowCanvas(aaiCanvas);

    memcpy(g_aaiCanvas, aaiCanvas, sizeof(g_aaiCanvas));

    return ERROR_SUCCESS;
}

int SHOW_ShowTip(char *szTip)
{
    int aaiCanvas[SHOW_ROW][SHOW_COL];
    uint uiLen;

    memcpy(aaiCanvas, g_aaiCanvas, sizeof(aaiCanvas));

    uiLen = strlen(szTip);
    for(int y = SHOW_ROW - 1; y < SHOW_ROW; y++)
    {
        for(int x = 0; x < SHOW_COL; x++)
        {
            if (uiLen > x)
            {
                aaiCanvas[y][x] = szTip[x];
            }
            else
            {
                aaiCanvas[y][x] = 0;
            }
        }
    }

    _show_ShowCanvas(aaiCanvas);

    memcpy(g_aaiCanvas, aaiCanvas, sizeof(g_aaiCanvas));

    return ERROR_SUCCESS;
}

void _show_SetCursorVisible(int iVisible)
{
    CONSOLE_CURSOR_INFO stCursorInfo;
    GetConsoleCursorInfo(g_hConsoleOut, &stCursorInfo);
    stCursorInfo.bVisible = (BOOL) iVisible;
    SetConsoleCursorInfo(g_hConsoleOut, &stCursorInfo);
}

int SHOW_Init()
{
    int iErrCode;

    g_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (NULL == g_hConsoleOut)
    {
        return ERROR_FAILED;
    }

    _show_SetCursorVisible(0);

    iErrCode = _show_InitCanvas();

    return iErrCode;
}

void SHOW_Exit()
{
    _show_GotoXY(0, SHOW_ROW);
    _show_SetCursorVisible(1);

    g_hConsoleOut = NULL;
}
