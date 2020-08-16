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
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
 * 0 ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
 * 1 ©¦....................©¦
 * 2 ©¦....................©¦  ©°©¤©¤©¤©¤©´
 * 3 ©¦....................©¦  ©¦........©¦
 * 4 ©¦....................©¦  ©¦........©¦
 * 5 ©¦....................©¦  ©¦........©¦
 * 6 ©¦....................©¦  ©¦........©¦
 * 7 ©¦....................©¦  ©¸©¤©¤©¤©¤©¼
 * 8 ©¦....................©¦
 * 9 ©¦....................©¦
 * 0 ©¦....................©¦    Score:
 * 1 ©¦....................©¦    0
 * 2 ©¦....................©¦
 * 3 ©¦....................©¦
 * 4 ©¦....................©¦    Level:
 * 5 ©¦....................©¦    1
 * 6 ©¦....................©¦
 * 7 ©¦....................©¦
 * 8 ©¦....................©¦
 * 9 ©¦....................©¦
 * 0 ©¦....................©¦
 * 1 ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
 * ©°©¤©´
 * ©¦  ©¦
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

#define SHOW_CANVAS_ROW ((DATA_MAP_ROW) + 2)
#define SHOW_CANVAS_COL ((DATA_MAP_COL) + (DATA_TETRIS_COL) + 5)
#define SHOW_CANVAS_RXC (SHOW_CANVAS_ROW * SHOW_CANVAS_COL)

#define SHOW_MAP_X_MIN 0
#define SHOW_MAP_X_MAX ((SHOW_MAP_X_MIN) + (DATA_MAP_COL) + 1)
#define SHOW_MAP_Y_MIN 0
#define SHOW_MAP_Y_MAX ((SHOW_MAP_Y_MIN) + (DATA_MAP_ROW) + 1)

#define SHOW_NEXT_X_MIN ((SHOW_MAP_X_MAX) + 2)
#define SHOW_NEXT_X_MAX ((SHOW_NEXT_X_MIN) + (DATA_TETRIS_COL) + 1)
#define SHOW_NEXT_Y_MIN 2
#define SHOW_NEXT_Y_MAX ((SHOW_NEXT_Y_MIN) + (DATA_TETRIS_ROW) + 1)

#define SHOW_SCORE_Y (SHOW_NEXT_Y_MAX + 3)
#define SHOW_SCOREV_Y (SHOW_SCORE_Y + 1)

#define SHOW_LEVEL_Y (SHOW_SCORE_Y + 3)
#define SHOW_LEVELV_Y (SHOW_LEVEL_Y + 1)

#define SHOW_CANVAS_INDEX(y, x) DATA_INDEX(SHOW_CANVAS_COL, y, x)

#define SHOW_SCORE_TEXT "Score:"
#define SHOW_LEVEL_TEXT "Level:"

HANDLE g_hShowConsoleOut = NULL;
int g_iShowCursorVisible = 0;

int g_aiShowMapCanvas[DATA_MAP_RXC];
int g_aiShowNextCanvas[DATA_TETRIS_RXC];
int g_iShowScore = 0;


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

void show_Goto(ushort y, ushort x)
{
    COORD pos;

    pos.Y = y;
    pos.X = x;

    SetConsoleCursorPosition(g_hShowConsoleOut, pos);

    return;
}

void show_ShowPixel(int pixel, ushort y, ushort x)
{
    CONSOLE_SCREEN_BUFFER_INFO stScreenInfo;
    show_Goto(y, x * SHOW_PIXEL_LEN);

    GetConsoleScreenBufferInfo(g_hShowConsoleOut, &stScreenInfo);

    SetConsoleTextAttribute(g_hShowConsoleOut, SHOW_COLOR(pixel));
    printf("%s", show_Sharp(pixel));
    SetConsoleTextAttribute(g_hShowConsoleOut, stScreenInfo.wAttributes);

    return;
}

int show_ShowPixels(int *piDst, int *piSrc, RECT_S *pstPos)
{
    int index, col;
    CONSOLE_SCREEN_BUFFER_INFO stInfo;

    assert(NULL != piSrc);
    assert(NULL != pstPos);

    GetConsoleScreenBufferInfo(g_hShowConsoleOut, &stInfo);

    col = pstPos->r - pstPos->l;
    for(int y = pstPos->t; y < pstPos->b; y++)
    {
        for(int x = pstPos->l; x < pstPos->r; x++)
        {
            index = DATA_INDEX(col, y - pstPos->t, x - pstPos->l);
            if (NULL != piDst && piDst[index] == piSrc[index])
            {
                continue;
            }

            show_ShowPixel(piSrc[index], y, x);
            if (NULL != piDst)
            {
                piDst[index] = piSrc[index];
            }
        }
    }

    SetConsoleCursorPosition(g_hShowConsoleOut, stInfo.dwCursorPosition);

    return ERROR_SUCCESS;
}

int show_ShowPixelsForce(int *piDst, int *piSrc, RECT_S *pstPos)
{
    int index, col;
    CONSOLE_SCREEN_BUFFER_INFO stInfo;

    assert(NULL != piSrc);
    assert(NULL != pstPos);

    GetConsoleScreenBufferInfo(g_hShowConsoleOut, &stInfo);

    col = pstPos->r - pstPos->l;
    for(int y = pstPos->t; y < pstPos->b; y++)
    {
        for(int x = pstPos->l; x < pstPos->r; x++)
        {
            index = DATA_INDEX(col, y - pstPos->t, x - pstPos->l);
            show_ShowPixel(piSrc[index], y, x);
            if (NULL != piDst)
            {
                piDst[index] = piSrc[index];
            }
        }
    }

    SetConsoleCursorPosition(g_hShowConsoleOut, stInfo.dwCursorPosition);

    return ERROR_SUCCESS;
}

int show_ShowText(char *pcSrc, RECT_S *pstPos)
{
    char acFormat[12] = "%%-%ds";
    CONSOLE_SCREEN_BUFFER_INFO stScreenInfo;

    assert(NULL != pcSrc);
    assert(NULL != pstPos);
    assert(pstPos->r > pstPos->l);

    acFormat[0] = 0;
    snprintf(acFormat, sizeof(acFormat), "%%-%ds", (pstPos->r - pstPos->l) * SHOW_PIXEL_LEN);

    GetConsoleScreenBufferInfo(g_hShowConsoleOut, &stScreenInfo);

    show_Goto(pstPos->t, pstPos->l * SHOW_PIXEL_LEN);
    SetConsoleTextAttribute(g_hShowConsoleOut, SHOW_COLOR_DEFAULT);
    printf(acFormat, pcSrc);
    SetConsoleTextAttribute(g_hShowConsoleOut, stScreenInfo.wAttributes);
    show_Goto(stScreenInfo.dwCursorPosition.Y, stScreenInfo.dwCursorPosition.X);

    return ERROR_SUCCESS;
}

int show_ShowScoreText(void)
{
    RECT_S stPos =
            {
            SHOW_NEXT_X_MIN + 1,
            SHOW_SCORE_Y,
            SHOW_NEXT_X_MAX,
            SHOW_SCORE_Y + 1
            };

    return show_ShowText(SHOW_SCORE_TEXT, &stPos);
}

int show_ShowLevelText(void)
{
    RECT_S stPos =
            {
            SHOW_NEXT_X_MIN + 1,
            SHOW_LEVEL_Y,
            SHOW_NEXT_X_MAX,
            SHOW_LEVEL_Y + 1
            };

    return show_ShowText(SHOW_LEVEL_TEXT, &stPos);
}

int show_ShowScoreForce(int iScore)
{
    char acScore[8];
    RECT_S stPos =
            {
            SHOW_NEXT_X_MIN + 1,
            SHOW_SCOREV_Y,
            SHOW_NEXT_X_MAX,
            SHOW_SCOREV_Y + 1
            };

    g_iShowScore = iScore;
    acScore[0] = 0;
    snprintf(acScore, sizeof(acScore), "%d", iScore);
    show_ShowText(acScore, &stPos);

    return ERROR_SUCCESS;
}

int show_InitCanvas(void)
{
    int x, y;
    int aiShowCanvas[SHOW_CANVAS_RXC];
    RECT_S stCanvasPos = {0, 0, SHOW_CANVAS_COL, SHOW_CANVAS_ROW};

    system("cls");

    for (y = 0; y < SHOW_CANVAS_ROW; y++)
    {
        for (x = 0; x < SHOW_CANVAS_COL; x++)
        {
            aiShowCanvas[SHOW_CANVAS_INDEX(y, x)] = SHOW_TABS_PIXEL(SHOW_SHARP_S);
        }
    }

    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_MAP_Y_MIN, SHOW_MAP_X_MIN)] = SHOW_TABS_PIXEL(SHOW_SHARP_LT);
    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_MAP_Y_MAX, SHOW_MAP_X_MIN)] = SHOW_TABS_PIXEL(SHOW_SHARP_LB);
    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_MAP_Y_MIN, SHOW_MAP_X_MAX)] = SHOW_TABS_PIXEL(SHOW_SHARP_RT);
    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_MAP_Y_MAX, SHOW_MAP_X_MAX)] = SHOW_TABS_PIXEL(SHOW_SHARP_RB);
    for (y = SHOW_MAP_Y_MIN + 1; y < SHOW_MAP_Y_MAX; y++)
        aiShowCanvas[SHOW_CANVAS_INDEX(y, SHOW_MAP_X_MIN)] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (y = SHOW_MAP_Y_MIN + 1; y < SHOW_MAP_Y_MAX; y++)
        aiShowCanvas[SHOW_CANVAS_INDEX(y, SHOW_MAP_X_MAX)] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (x = SHOW_MAP_X_MIN + 1; x < SHOW_MAP_X_MAX; x++)
        aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_MAP_Y_MIN, x)] = SHOW_TABS_PIXEL(SHOW_SHARP_H);
    for (x = SHOW_MAP_X_MIN + 1; x < SHOW_MAP_X_MAX; x++)
        aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_MAP_Y_MAX, x)] = SHOW_TABS_PIXEL(SHOW_SHARP_H);

    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_NEXT_Y_MIN, SHOW_NEXT_X_MIN)] = SHOW_TABS_PIXEL(SHOW_SHARP_LT);
    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_NEXT_Y_MAX, SHOW_NEXT_X_MIN)] = SHOW_TABS_PIXEL(SHOW_SHARP_LB);
    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_NEXT_Y_MIN, SHOW_NEXT_X_MAX)] = SHOW_TABS_PIXEL(SHOW_SHARP_RT);
    aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_NEXT_Y_MAX, SHOW_NEXT_X_MAX)] = SHOW_TABS_PIXEL(SHOW_SHARP_RB);
    for (y = SHOW_NEXT_Y_MIN + 1; y < SHOW_NEXT_Y_MAX; y++)
        aiShowCanvas[SHOW_CANVAS_INDEX(y, SHOW_NEXT_X_MIN)] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (y = SHOW_NEXT_Y_MIN + 1; y < SHOW_NEXT_Y_MAX; y++)
        aiShowCanvas[SHOW_CANVAS_INDEX(y, SHOW_NEXT_X_MAX)] = SHOW_TABS_PIXEL(SHOW_SHARP_V);
    for (x = SHOW_NEXT_X_MIN + 1; x < SHOW_NEXT_X_MAX; x++)
        aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_NEXT_Y_MIN, x)] = SHOW_TABS_PIXEL(SHOW_SHARP_H);
    for (x = SHOW_NEXT_X_MIN + 1; x < SHOW_NEXT_X_MAX; x++)
        aiShowCanvas[SHOW_CANVAS_INDEX(SHOW_NEXT_Y_MAX, x)] = SHOW_TABS_PIXEL(SHOW_SHARP_H);

    show_ShowPixelsForce(NULL, aiShowCanvas, &stCanvasPos);

    show_ShowScoreText();
    show_ShowLevelText();
    show_ShowScoreForce(0);

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

int SHOW_ShowMap(int aiMap[DATA_MAP_RXC])
{
    RECT_S stPos =
            {
            SHOW_MAP_X_MIN + 1,
            SHOW_MAP_Y_MIN + 1,
            SHOW_MAP_X_MAX,
            SHOW_MAP_Y_MAX
            };

    show_ShowPixels(g_aiShowMapCanvas, aiMap, &stPos);

    return ERROR_SUCCESS;
}

int SHOW_ShowNext(int aiNext[DATA_TETRIS_RXC])
{
    RECT_S stPos =
            {
            SHOW_NEXT_X_MIN + 1,
            SHOW_NEXT_Y_MIN + 1,
            SHOW_NEXT_X_MAX,
            SHOW_NEXT_Y_MAX
            };

    show_ShowPixels(g_aiShowNextCanvas, aiNext, &stPos);

    return ERROR_SUCCESS;
}

int SHOW_ShowScore(int iScore)
{
    char acScore[8];
    RECT_S stPos =
            {
            SHOW_NEXT_X_MIN + 1,
            SHOW_SCOREV_Y,
            SHOW_NEXT_X_MAX,
            SHOW_SCOREV_Y + 1
            };

    if (g_iShowScore == iScore)
    {
        return ERROR_SUCCESS;
    }

    g_iShowScore = iScore;
    acScore[0] = 0;
    snprintf(acScore, sizeof(acScore), "%d", iScore);
    show_ShowText(acScore, &stPos);

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
    show_Goto(SHOW_CANVAS_ROW, 0);

    return iErrCode;
}

void SHOW_Exit()
{
    show_Goto(SHOW_CANVAS_ROW, 0);
    show_SetCursorVisible(g_iShowCursorVisible);

    g_hShowConsoleOut = NULL;
}

#ifdef __cplusplus
}
#endif
