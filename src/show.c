/*
 * show.c
 *
 *  Created on: 2018-09-02
 *      Author: Wind
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>

#include "base.h"
#include "main.h"
#include "map.h"
#include "data.h"
#include "show.h"

/*
 * ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
 * ©¦....................©¦ ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 * ©¦....................©¦ ©¦........©¦
 * ©¦....................©¦ ©¦........©¦
 * ©¦....................©¦ ©¦........©¦
 * ©¦....................©¦ ©¦........©¦
 * ©¦....................©¦ ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 * ©¦....................©¦  Score
 * ©¦....................©¦  0
 * ©¦....................©¦
 * ©¦....................©¦  Level
 * ©¦....................©¦  1
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¦....................©¦
 * ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼

 * ©°©¤©´
 * ©¦ ©¦
 * ©¸©¤©¼
 */
#define SHOW_CANVAS_S  "  "
#define SHOW_CANVAS_H  "©¤"
#define SHOW_CANVAS_V  "©¦"
#define SHOW_CANVAS_LT "©°"
#define SHOW_CANVAS_LB "©¸"
#define SHOW_CANVAS_RT "©´"
#define SHOW_CANVAS_RB "©¼"
#define SHOW_CANVAS_INVALID "  "
#define SHOW_CANVAS_POINT   "¡ö"

#define SHOW_PIXEL_LEN 2

#define SHOW_ROW (DATA_MAP_ROW + 2)
#define SHOW_COL (DATA_MAP_COL + DATA_NEXT_COL + 5)

#define SHOW_MAP_X_MIN 0
#define SHOW_MAP_X_MAX (SHOW_MAP_X_MIN + DATA_MAP_COL + 1)
#define SHOW_MAP_Y_MIN 0
#define SHOW_MAP_Y_MAX (SHOW_MAP_Y_MIN + DATA_MAP_ROW + 1)

#define SHOW_NEXT_X_MIN (DATA_MAP_COL + 3)
#define SHOW_NEXT_X_MAX (SHOW_NEXT_X_MIN + DATA_NEXT_COL + 1)
#define SHOW_NEXT_Y_MIN 1
#define SHOW_NEXT_Y_MAX (SHOW_NEXT_Y_MIN + DATA_NEXT_ROW + 1)

HANDLE g_hShowConsoleOut = NULL;
int g_aaiShowCanvas[SHOW_ROW][SHOW_COL] = {0};
int g_iShowCursorVisible = 0;

char *show_Sharp(int iPixel)
{
    switch(SHOW_SHARP(iPixel))
    {
    case SHOW_SHARP_S:
        return SHOW_CANVAS_S;
    case SHOW_SHARP_H:
        return SHOW_CANVAS_H;
    case SHOW_SHARP_V:
        return SHOW_CANVAS_V;
    case SHOW_SHARP_LT:
        return SHOW_CANVAS_LT;
    case SHOW_SHARP_LB:
        return SHOW_CANVAS_LB;
    case SHOW_SHARP_RT:
        return SHOW_CANVAS_RT;
    case SHOW_SHARP_RB:
        return SHOW_CANVAS_RB;
    case SHOW_SHARP_INVALID:
        return SHOW_CANVAS_INVALID;
    case SHOW_SHARP_POINT:
    default:
        return SHOW_CANVAS_POINT;
    }
}

void show_GotoXY(ushort x, ushort y)
{
    COORD pos;

    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(g_hShowConsoleOut, pos);

    return;
}

void show_ShowPixel(int pixel, ushort x, ushort y)
{
    show_GotoXY(x * SHOW_PIXEL_LEN, y);

    SetConsoleTextAttribute(g_hShowConsoleOut, SHOW_COLOR(pixel));
    printf("%s", show_Sharp(pixel));
    SetConsoleTextAttribute(g_hShowConsoleOut, SHOW_COLOR_DEFAULT);

    return;
}

int show_ShowCanvas(int aaiCanvas[SHOW_ROW][SHOW_COL], int iForce)
{
    CONSOLE_SCREEN_BUFFER_INFO stInfo;

    GetConsoleScreenBufferInfo(g_hShowConsoleOut, &stInfo);

    for(int y = 0; y < SHOW_ROW; y++)
    {
        for(int x = 0; x < SHOW_COL; x++)
        {
            if (g_aaiShowCanvas[y][x] == aaiCanvas[y][x] && !iForce)
            {
                continue;
            }

            show_ShowPixel(aaiCanvas[y][x], x, y);
        }
    }

    SetConsoleCursorPosition(g_hShowConsoleOut, stInfo.dwCursorPosition);

    return ERROR_SUCCESS;
}

int show_InitCanvas(void)
{
    int x, y;
    system("cls");

    for (y = 0; y < SHOW_ROW; y++)
    {
        for (x = 0; x < SHOW_COL; x++)
        {
            g_aaiShowCanvas[y][x] = SHOW_SHARP_S;
        }
    }

    g_aaiShowCanvas[SHOW_MAP_Y_MIN][SHOW_MAP_X_MIN] = SHOW_TABS_PIXEL(SHOW_SHARP_LT);
    g_aaiShowCanvas[SHOW_MAP_Y_MAX][SHOW_MAP_X_MIN] = SHOW_TABS_PIXEL(SHOW_SHARP_LB);
    g_aaiShowCanvas[SHOW_MAP_Y_MIN][SHOW_MAP_X_MAX] = SHOW_TABS_PIXEL(SHOW_SHARP_RT);
    g_aaiShowCanvas[SHOW_MAP_Y_MAX][SHOW_MAP_X_MAX] = SHOW_TABS_PIXEL(SHOW_SHARP_RB);
    for (y = SHOW_MAP_Y_MIN + 1; y < SHOW_MAP_Y_MAX; y++) g_aaiShowCanvas[y][SHOW_MAP_X_MIN] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (y = SHOW_MAP_Y_MIN + 1; y < SHOW_MAP_Y_MAX; y++) g_aaiShowCanvas[y][SHOW_MAP_X_MAX] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (x = SHOW_MAP_X_MIN + 1; x < SHOW_MAP_X_MAX; x++) g_aaiShowCanvas[SHOW_MAP_Y_MIN][x] = SHOW_TABS_PIXEL(SHOW_SHARP_H);
    for (x = SHOW_MAP_X_MIN + 1; x < SHOW_MAP_X_MAX; x++) g_aaiShowCanvas[SHOW_MAP_Y_MAX][x] = SHOW_TABS_PIXEL(SHOW_SHARP_H);

    g_aaiShowCanvas[SHOW_NEXT_Y_MIN][SHOW_NEXT_X_MIN] = SHOW_TABS_PIXEL(SHOW_SHARP_LT);
    g_aaiShowCanvas[SHOW_NEXT_Y_MAX][SHOW_NEXT_X_MIN] = SHOW_TABS_PIXEL(SHOW_SHARP_LB);
    g_aaiShowCanvas[SHOW_NEXT_Y_MIN][SHOW_NEXT_X_MAX] = SHOW_TABS_PIXEL(SHOW_SHARP_RT);
    g_aaiShowCanvas[SHOW_NEXT_Y_MAX][SHOW_NEXT_X_MAX] = SHOW_TABS_PIXEL(SHOW_SHARP_RB);
    for (y = SHOW_NEXT_Y_MIN + 1; y < SHOW_NEXT_Y_MAX; y++) g_aaiShowCanvas[y][SHOW_NEXT_X_MIN] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (y = SHOW_NEXT_Y_MIN + 1; y < SHOW_NEXT_Y_MAX; y++) g_aaiShowCanvas[y][SHOW_NEXT_X_MAX] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (x = SHOW_NEXT_X_MIN + 1; x < SHOW_NEXT_X_MAX; x++) g_aaiShowCanvas[SHOW_NEXT_Y_MIN][x] = SHOW_TABS_PIXEL(SHOW_SHARP_H);
    for (x = SHOW_NEXT_X_MIN + 1; x < SHOW_NEXT_X_MAX; x++) g_aaiShowCanvas[SHOW_NEXT_Y_MAX][x] = SHOW_TABS_PIXEL(SHOW_SHARP_H);

    show_ShowCanvas(g_aaiShowCanvas, 1);

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

int SHOW_ShowMap(int aaiMap[DATA_MAP_ROW][DATA_MAP_COL])
{
    int x, y;
    int aaiCanvas[SHOW_ROW][SHOW_COL];

    memcpy(aaiCanvas, g_aaiShowCanvas, sizeof(aaiCanvas));

    for(y = 0; y < DATA_MAP_ROW && y < SHOW_ROW; y++)
    {
        for(x = 0; x < DATA_MAP_COL && x < SHOW_COL; x++)
        {
            aaiCanvas[y + SHOW_MAP_Y_MIN + 1][x + SHOW_MAP_X_MIN + 1] = aaiMap[y][x];
        }
        assert(x + SHOW_MAP_X_MIN + 1 == SHOW_MAP_X_MAX);
    }
    assert(y + SHOW_MAP_Y_MIN + 1 == SHOW_MAP_Y_MAX);

    show_ShowCanvas(aaiCanvas, 0);

    memcpy(g_aaiShowCanvas, aaiCanvas, sizeof(g_aaiShowCanvas));

    return ERROR_SUCCESS;
}

int SHOW_ShowNext(int aaiNext[DATA_NEXT_ROW][DATA_NEXT_COL])
{
    int x, y;
    int aaiCanvas[SHOW_ROW][SHOW_COL];

    memcpy(aaiCanvas, g_aaiShowCanvas, sizeof(aaiCanvas));

    for(y = 0; y < DATA_NEXT_ROW; y++)
    {
        for(x = 0; x < DATA_NEXT_COL; x++)
        {
            aaiCanvas[y + SHOW_NEXT_Y_MIN + 1][x + SHOW_NEXT_X_MIN + 1] = aaiNext[y][x];
        }
        assert(x + SHOW_NEXT_X_MIN + 1 == SHOW_NEXT_X_MAX);
    }
    assert(y + SHOW_NEXT_Y_MIN + 1 == SHOW_NEXT_Y_MAX);

    show_ShowCanvas(aaiCanvas, 0);

    memcpy(g_aaiShowCanvas, aaiCanvas, sizeof(g_aaiShowCanvas));

    return ERROR_SUCCESS;
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

#ifdef __cplusplus
}
#endif
