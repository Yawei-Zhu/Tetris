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


int g_aaaiDataAllTetris[DATA_TETRIS_NUM][4][2] =
    {
            {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
            {{0, 1}, {0, 2}, {1, 2}, {2, 2}},
            {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
            {{1, 1}, {1, 2}, {2, 1}, {2, 2}},
            {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
            {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
            {{1, 2}, {2, 1}, {2, 2}, {2, 3}},
    };

int g_aiDataAllTetrisColor[DATA_TETRIS_NUM] =
        {
                SHOW_COLOR_HY,
                SHOW_COLOR_BLUE,
                SHOW_COLOR_GREEN,
                SHOW_COLOR_LG,
                SHOW_COLOR_RED,
                SHOW_COLOR_PURPLE,
                SHOW_COLOR_YELLOW,
        };

int g_aiDataAllTetrisProbability[DATA_TETRIS_NUM] =
        {
                1,
                1,
                1,
                1,
                1,
                1,
                1,
        };


DATA_RUN_S g_stDataRun;

rpint DATA_GetMapCanvas(void)
{
    return g_stDataRun.aaiMapCanvas;
}

int DATA_SetMapCanvas(int ppiCanvas[DATA_MAP_ROW][DATA_MAP_COL])
{
    memcpy(g_stDataRun.aaiMapCanvas, ppiCanvas, sizeof(g_stDataRun.aaiMapCanvas));

    return ERROR_SUCCESS;
}

DATA_TETRIS_S *DATA_GetNextTetris(void)
{
    return &g_stDataRun.stNextTetris;
}

int   DATA_SetNextTetris(DATA_TETRIS_S *pstTetris)
{
    memcpy(&g_stDataRun.stNextTetris, pstTetris,
           sizeof(g_stDataRun.stNextTetris));

    return ERROR_SUCCESS;
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

int *DATA_GetCurrTetrisPosition(void)
{
    return g_stDataRun.aiCurrTetrisPosition;
}

int   DATA_SetCurrTetrisPosition(int aiPosition[2])
{
    memcpy(g_stDataRun.aiCurrTetrisPosition, aiPosition,
           sizeof(g_stDataRun.aiCurrTetrisPosition));
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

int DATA_ResetTimeCount()
{
    return g_stDataRun.iTimeCount = 0;
}

int  DATA_UpdateTimeCount()
{
    return ++g_stDataRun.iTimeCount;
}

int  DATA_PeekTimeCount()
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
    int iSum, iRand, iIndex;

    return rand() % DATA_TETRIS_NUM;
    iSum = 0;
    for (int i = 0; i < DATA_TETRIS_NUM; i++)
    {
        iSum += g_aiDataAllTetrisProbability[i];
    }

    iRand = rand() % iSum;

    iIndex = 0;
    for (int i = 0; i < DATA_TETRIS_NUM; i++)
    {
        if (iRand < g_aiDataAllTetrisProbability[i])
        {
            iIndex = i;
            break;
        }
        iRand -= g_aiDataAllTetrisProbability[i];
    }

    if (g_aiDataAllTetrisProbability[iIndex] > 1)
    {
        g_aiDataAllTetrisProbability[iIndex] >>= 1;
    }
    else
    {
        for (int i = 0; i < DATA_TETRIS_NUM; i++)
        {
            if (iIndex != i)
            {
                g_aiDataAllTetrisProbability[i] <<= 1;
                break;
            }
        }
    }

    return iIndex;
}

void DATA_GetRandTetris(DATA_TETRIS_S *pstTetris)
{
    int iRand;
    assert(NULL != pstTetris);

    memset(pstTetris, 0, sizeof(DATA_TETRIS_S));

    iRand = data_Rand();
    memcpy(pstTetris->aaiTetris,
            g_aaaiDataAllTetris[iRand],
            sizeof(pstTetris->aaiTetris));

    pstTetris->iTetrisPixel = SHOW_POINT_PIXEL(g_aiDataAllTetrisColor[iRand]);

    return;
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
