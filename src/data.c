/*
 * data.c
 *
 *  Created on: 2019-03-03
 *      Author: Wind
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "base.h"
#include "main.h"
#include "tlv.h"
#include "map.h"
#include "data.h"
#include "show.h"

DATA_TETRIS_S g_astDataAllTetris[DATA_TETRIS_NUM] =
{
        {{{2, 0}, {2, 1}, {2, 2}, {2, 3}}, SHOW_POINT_PIXEL(SHOW_COLOR_HY)},
        {{{1, 0}, {2, 0}, {2, 1}, {2, 2}}, SHOW_POINT_PIXEL(SHOW_COLOR_BLUE)},
        {{{1, 0}, {2, 0}, {1, 1}, {1, 2}}, SHOW_POINT_PIXEL(SHOW_COLOR_GREEN)},
        {{{1, 1}, {2, 1}, {1, 2}, {2, 2}}, SHOW_POINT_PIXEL(SHOW_COLOR_LG)},
        {{{1, 0}, {1, 1}, {2, 1}, {2, 2}}, SHOW_POINT_PIXEL(SHOW_COLOR_RED)},
        {{{2, 0}, {1, 1}, {2, 1}, {1, 2}}, SHOW_POINT_PIXEL(SHOW_COLOR_PURPLE)},
        {{{2, 1}, {1, 2}, {2, 2}, {3, 2}}, SHOW_POINT_PIXEL(SHOW_COLOR_YELLOW)},
};
int g_aiDataSameRand[2];
int g_iDataSameRandIndex = 0;
DATA_RUN_S g_stDataRun;

int *DATA_GetMapCanvas(void)
{
    return g_stDataRun.aiMapCanvas;
}

int DATA_SetMapCanvas(int piCanvas[DATA_MAP_RXC])
{
    memcpy(g_stDataRun.aiMapCanvas, piCanvas, sizeof(g_stDataRun.aiMapCanvas));

    return ERROR_SUCCESS;
}

DATA_TETRIS_S *DATA_GetNextTetris(void)
{
    return &g_stDataRun.stNextTetris;
}

DATA_TETRIS_S *DATA_UpdateNextTetris(void)
{
    memcpy(&g_stDataRun.stNextTetris, DATA_GetRandTetris(),
           sizeof(g_stDataRun.stNextTetris));

    return &g_stDataRun.stNextTetris;
}

DATA_TETRIS_S *DATA_GetCurrTetris(void)
{
    return &g_stDataRun.stCurrTetris;
}

int   DATA_SetCurrTetris(DATA_TETRIS_S *pstTetris)
{
    memcpy(&g_stDataRun.stCurrTetris, pstTetris,
           sizeof(g_stDataRun.stCurrTetris));
    return ERROR_SUCCESS;
}

POINT_S *DATA_GetCurrTetrisPos(void)
{
    return &g_stDataRun.stCurrTetrisPos;
}

int   DATA_SetCurrTetrisPos(POINT_S *pstPosition)
{
    memcpy(&g_stDataRun.stCurrTetrisPos,
           pstPosition,
           sizeof(g_stDataRun.stCurrTetrisPos));
    return ERROR_SUCCESS;
}

MAP_STATE_E  DATA_GetCurrState(void)
{
    return g_stDataRun.enCurrState;
}

int DATA_SetCurrState(MAP_STATE_E enState)
{
    g_stDataRun.enCurrState = enState;
    return ERROR_SUCCESS;
}

DATA_RMROW_S *DATA_GetRmRow(void)
{
    return &g_stDataRun.stRmRow;
}

int DATA_SetRmRow(DATA_RMROW_S *pstRmRow)
{
    memcpy(&g_stDataRun.stRmRow, pstRmRow, sizeof(g_stDataRun.stRmRow));

    return ERROR_SUCCESS;
}

int  DATA_ResetScore(void)
{
    return g_stDataRun.iScore = 0;
}

int  DATA_IncreaseScore(int iRow)
{
    if (iRow <= 0)
    {
        return g_stDataRun.iScore = 0;
    }
    else
    {
        return g_stDataRun.iScore += iRow * iRow;
    }
}

int  DATA_PeekScore(void)
{
    return g_stDataRun.iScore;
}

int DATA_ResetTimeCount(void)
{
    return g_stDataRun.iTimeCount = 0;
}

int  DATA_UpdateTimeCount(void)
{
    return ++g_stDataRun.iTimeCount;
}

int  DATA_PeekTimeCount(void)
{
    return g_stDataRun.iTimeCount;
}

void DATA_SetRandSeed(uint uiSeed)
{
    srand(uiSeed);
    return;
}

int data_Rand()
{
    int iCount, iRand, iOld, iIndex;

    iCount = ARRAY_SIZE(g_aiDataSameRand);

    iOld = g_aiDataSameRand[0];
    if (iOld >= 0)
    {
        for (iIndex = 1; iIndex < iCount; iIndex++)
        {
            if (iOld != g_aiDataSameRand[iIndex])
            {
                iOld = -1;
                break;
            }
        }
    }

    if (iOld >= 0)
    {
        iRand = rand() % (DATA_TETRIS_NUM - 1);
        if (iRand >= iOld)
        {
            iRand++;
        }
    }
    else
    {
        iRand = rand() % DATA_TETRIS_NUM;
    }

    g_aiDataSameRand[g_iDataSameRandIndex] = iRand;
    g_iDataSameRandIndex++;
    if (g_iDataSameRandIndex >= iCount)
    {
        g_iDataSameRandIndex = 0;
    }

    return iRand;
}

DATA_TETRIS_S *DATA_GetRandTetris(void)
{
    int iRand;

    iRand = data_Rand();

    return g_astDataAllTetris + iRand;
}

DATA_RUN_S *DATA_GetRun(void)
{
    return &g_stDataRun;
}

int  DATA_Save(void)
{
//    char acBuff[TLV_BUFFLEN_MAX];
//    uint uiTotalLen;
//    uint uiDataLen;

    return ERROR_SUCCESS;
}

int  DATA_Init(void)
{
    memset(&g_stDataRun, 0, sizeof(g_stDataRun));
    memset(&g_aiDataSameRand, -1, sizeof(g_aiDataSameRand));
    g_iDataSameRandIndex = 0;
    return ERROR_SUCCESS;
}

void DATA_Exit(void)
{
    DATA_Save();
    memset(&g_stDataRun, 0, sizeof(g_stDataRun));
    return;
}

#ifdef __cplusplus
}
#endif
