/*
 * data.c
 *
 *  Created on: 2019年3月3日
 *      Author: Wind
 */


#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "base.h"
#include "main.h"
#include "map.h"
#include "data.h"
#include "log.h"


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

DATA_RUN_S g_stDataRun;

rpint DATA_GetMapCanvas(void)
{
    return g_stDataRun.aaiMapCanvas;
}

int DATA_SetMapCanvas(int ppiCanvas[MAP_ROW][MAP_COL])
{
    memcpy(g_stDataRun.aaiMapCanvas, ppiCanvas, sizeof(g_stDataRun.aaiMapCanvas));

    return ERROR_SUCCESS;
}

rpint DATA_GetNextTetris(void)
{
    return g_stDataRun.aaiNextTetris;
}

int   DATA_SetNextTetris(int ppiTetris[4][2])
{
    memcpy(g_stDataRun.aaiNextTetris, ppiTetris,
           sizeof(g_stDataRun.aaiNextTetris));

    return ERROR_SUCCESS;
}

rpint DATA_GetCurrTetris(void)
{
    return g_stDataRun.aaiCurrTetris;
}

int   DATA_SetCurrTetris(int ppiTetris[4][2])
{
    memcpy(g_stDataRun.aaiCurrTetris, ppiTetris,
           sizeof(g_stDataRun.aaiCurrTetris));
    return ERROR_SUCCESS;
}

int *DATA_GetCurrTetrisPosition(void)
{
    return g_stDataRun.aiCurrTetrisPosition;
}

int   DATA_SetCurrTetrisPosition(int piPosition[2])
{
    memcpy(g_stDataRun.aiCurrTetrisPosition, piPosition,
           sizeof(g_stDataRun.aiCurrTetrisPosition));
    return ERROR_SUCCESS;
}

MAP_STATE_E  DATA_GetCurrState(void)
{
    return g_stDataRun.enCurrState;
}

int          DATA_SetCurrState(MAP_STATE_E enState)
{
    g_stDataRun.enCurrState = enState;
    return ERROR_SUCCESS;
}

rpint DATA_GetRandTetris(void)
{
    return g_aaaiDataAllTetris[rand() % DATA_TETRIS_NUM];
}

int  DATA_Save(void)
{
    return ERROR_SUCCESS;
}

int  DATA_Init(void)
{
    return ERROR_SUCCESS;
}

void DATA_Exit(void)
{
    return;
}
