/*
 * map.c
 *
 *  Created on: 2018-7-15
 *      Author: Wind
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "base.h"
#include "main.h"
#include "map.h"
#include "data.h"
#include "show.h"
#include "log.h"

typedef MAP_EVENT_E (*MAP_PROCEVENT_PF)(void);

void map_SetCurrTetris(DATA_TETRIS_S *pstTetris)
{
    POINT_S stPosition;
    int iMax;

    (void) DATA_SetCurrTetris(pstTetris);

    iMax = pstTetris->astTetris[0].y;
    for (int i = 1; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        if (iMax < pstTetris->astTetris[i].y)
        {
            iMax = pstTetris->astTetris[i].y;
        }
    }

    stPosition.y = 0 - iMax;
    stPosition.x = DATA_MAP_COL / 2 - 2;
    (void) DATA_SetCurrTetrisPos(&stPosition);

    return;
}

void map_Show(void)
{
    int aiMap[DATA_MAP_ROW * DATA_MAP_COL], aiNext[DATA_TETRIS_ROW * DATA_TETRIS_COL];
    int x, y;
    POINT_S *pstPosition;
    DATA_TETRIS_S *pstTetris;

    memcpy(aiMap, DATA_GetMapCanvas(), sizeof(aiMap));

    pstTetris = DATA_GetCurrTetris();
    pstPosition = DATA_GetCurrTetrisPos();
    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        y = pstTetris->astTetris[i].y + pstPosition->y;
        x = pstTetris->astTetris[i].x + pstPosition->x;
        if (0 <= y)
        {
            assert(0 <= y && y < DATA_MAP_ROW && 0 <= x && x < DATA_MAP_COL);
            aiMap[DATA_INDEX_MAP(y, x)] = pstTetris->iTetrisPixel;
        }
    }

    SHOW_ShowMap(aiMap);

    pstTetris = DATA_GetNextTetris();
    memset(aiNext, 0, sizeof(aiNext));
    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        y = pstTetris->astTetris[i].y;
        x = pstTetris->astTetris[i].x;
        aiNext[DATA_INDEX_NEXT(y, x)] = pstTetris->iTetrisPixel;
    }

    SHOW_ShowNext(aiNext);

    SHOW_ShowScore(DATA_PeekScore());

    return;
}

int map_RemoveRowMap(int aiMap[DATA_MAP_RXC])
{
    int iRemove, iCount;
    int *piMap;

    piMap = DATA_GetMapCanvas();
    iCount = 0;
    for (int y = DATA_MAP_ROW - 1; y >= 0; y--)
    {
        iRemove = 1;
        for (int x = 0; x < DATA_MAP_COL; x++)
        {
            aiMap[DATA_INDEX_MAP(y + iCount, x)] = piMap[DATA_INDEX_MAP(y, x)];
            if (SHOW_SHARP_INVALID != piMap[DATA_INDEX_MAP(y, x)])
            {
                iRemove = 0;
            }
        }

        if (iRemove)
        {
            iCount++;
        }
    }
    assert(0 < iCount);

    return iCount;
}

int map_RemoveRowCurrTetris(int aiMap[DATA_MAP_RXC])
{
    int iRemove, iCount, x, y, iCannot = 0;
    int *piRmRow;
    POINT_S *pstPosition;
    DATA_TETRIS_S *pstTetris;
    DATA_RMROW_S *pstRmRow;

    pstRmRow = DATA_GetRmRow();
    piRmRow = pstRmRow->aiRow;
    assert(0 < pstRmRow->iCount);

    pstTetris = DATA_GetCurrTetris();
    pstPosition = DATA_GetCurrTetrisPos();
    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        iRemove = 0;
        iCount = 0;
        y = pstTetris->astTetris[i].y + pstPosition->y;
        x = pstTetris->astTetris[i].x + pstPosition->x;
        for (int index = 0; index < pstRmRow->iCount; index++)
        {
            if (y == piRmRow[index])
            {
                iRemove = 1;
                break;
            }
            else if (y < piRmRow[index])
            {
                iCount++;
            }
        }

        if (!iRemove)
        {
            y += iCount;
            if (y >= 0)
            {
                aiMap[DATA_INDEX_MAP(y, x)] = pstTetris->iTetrisPixel;
            }

            if (y <= 0)
            {
                iCannot = 1;
            }
        }
    }

    return iCannot;
}

int map_RemoveRow(void)
{
    int iIsOver;
    int aiMap[DATA_MAP_RXC];

    memset(aiMap, 0, sizeof(aiMap));

    (void) map_RemoveRowMap(aiMap);

    iIsOver = map_RemoveRowCurrTetris(aiMap);

    (void) DATA_SetMapCanvas(aiMap);

    return iIsOver;
}

int map_TryRemoveCol(void)
{
    int aiMap[DATA_MAP_RXC];
    int iRemove, iLeft, iRight;
    int *piRmRow;
    DATA_RMROW_S *pstRmRow;

    memcpy(aiMap, DATA_GetMapCanvas(), sizeof(aiMap));
    pstRmRow = DATA_GetRmRow();
    piRmRow = pstRmRow->aiRow;
    assert(0 < pstRmRow->iCount);

    iRemove = 0;
    iLeft = (DATA_MAP_COL - 1) / 2;
    iRight = (DATA_MAP_COL) / 2;
    for (; iLeft >= 0; iLeft--, iRight++)
    {
        assert(DATA_MAP_COL > iRight);
        if (SHOW_SHARP_INVALID == aiMap[DATA_INDEX_MAP(piRmRow[0], iLeft)])
        {
            continue;
        }

        for (int i = 0; i < pstRmRow->iCount; i++)
        {
            assert(SHOW_SHARP_INVALID != aiMap[DATA_INDEX_MAP(piRmRow[i], iLeft)]);
            aiMap[DATA_INDEX_MAP(piRmRow[i], iLeft)] = SHOW_SHARP_INVALID;
            if (iRight != iLeft)
            {
                assert(SHOW_SHARP_INVALID != aiMap[DATA_INDEX_MAP(piRmRow[i], iRight)]);
                aiMap[DATA_INDEX_MAP(piRmRow[i], iRight)] = SHOW_SHARP_INVALID;
            }
        }
        iRemove = 1;
        break;
    }

    if (iRemove)
    {
        (void) DATA_SetMapCanvas(aiMap);
    }

    return iRemove;
}

MAP_EVENT_E map_ActionInit()
{
    int aiMap[DATA_MAP_RXC];
    DATA_TETRIS_S stTetris;

    memset(aiMap, 0, sizeof(aiMap));
    (void) DATA_SetMapCanvas(aiMap);
    (void) DATA_ResetTimeCount();

    memset(&stTetris, 0, sizeof(stTetris));
    map_SetCurrTetris(&stTetris);

    DATA_SetRandSeed(time(NULL));
    DATA_UpdateNextTetris();

    DATA_ResetScore();

    DATA_SetCurrState(MS_INIT);

    return ME_MAX;
}

MAP_EVENT_E map_ActionStart()
{
    map_SetCurrTetris(DATA_GetNextTetris());
    DATA_UpdateNextTetris();
    DATA_SetCurrState(MS_WAITING);

    return ME_MAX;
}

MAP_EVENT_E map_ActionDown()
{
    int x, y;
    int *piMap;
    POINT_S stPosition;
    DATA_TETRIS_S *pstTetris;

    if (MS_WAITING == DATA_GetCurrState())
    {
        if (MAIN_TIMECOUNT_WAIT > DATA_UpdateTimeCount())
        {
            return ME_MAX;
        }

        DATA_ResetTimeCount();
    }
    else
    {
        assert(MS_FAST == DATA_GetCurrState());

        if (MAIN_TIMECOUNT_FAST > DATA_UpdateTimeCount())
        {
            return ME_MAX;
        }
    }

    DATA_ResetTimeCount();

    memcpy(&stPosition, DATA_GetCurrTetrisPos(), sizeof(stPosition));
    stPosition.y++;

    pstTetris = DATA_GetCurrTetris();
    piMap = DATA_GetMapCanvas();
    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        y = pstTetris->astTetris[i].y + stPosition.y;
        x = pstTetris->astTetris[i].x + stPosition.x;

        if (DATA_MAP_ROW <= y ||
                (0 <= y && SHOW_SHARP_INVALID != piMap[DATA_INDEX_MAP(y, x)]))
        {
            return ME_LAND;
        }
    }

    DATA_SetCurrTetrisPos(&stPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionLeft()
{
    int x, y;
    int *piMap;
    POINT_S stPosition;
    DATA_TETRIS_S *pstTetris;

    memcpy(&stPosition, DATA_GetCurrTetrisPos(), sizeof(stPosition));
    stPosition.x--;

    pstTetris = DATA_GetCurrTetris();
    piMap = DATA_GetMapCanvas();
    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        y = pstTetris->astTetris[i].y + stPosition.y;
        x = pstTetris->astTetris[i].x + stPosition.x;

        if (0 > x || (0 <= y && SHOW_SHARP_INVALID != piMap[DATA_INDEX_MAP(y, x)]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetrisPos(&stPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionRight()
{
    int x, y;
    int *piMap;
    POINT_S stPosition;
    DATA_TETRIS_S *pstTetris;

    memcpy(&stPosition, DATA_GetCurrTetrisPos(), sizeof(stPosition));
    stPosition.x++;

    pstTetris = DATA_GetCurrTetris();
    piMap = DATA_GetMapCanvas();
    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        y = pstTetris->astTetris[i].y + stPosition.y;
        x = pstTetris->astTetris[i].x + stPosition.x;

        if (DATA_MAP_COL <= x ||
                (0 <= y && SHOW_SHARP_INVALID != piMap[DATA_INDEX_MAP(y, x)]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetrisPos(&stPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionClock()
{
    int x, y;
    int *piMap;
    POINT_S *pstPosition;
    DATA_TETRIS_S stTetris;

    memcpy(&stTetris, DATA_GetCurrTetris(), sizeof(stTetris));
    for (int i = 0; i < ARRAY_SIZE(stTetris.astTetris); i++)
    {
        y = stTetris.astTetris[i].y;
        x = stTetris.astTetris[i].x;

        stTetris.astTetris[i].y = x;
        stTetris.astTetris[i].x = 3 - y;
    }

    pstPosition = DATA_GetCurrTetrisPos();
    piMap = DATA_GetMapCanvas();
    for (int i = 0; i < ARRAY_SIZE(stTetris.astTetris); i++)
    {
        y = stTetris.astTetris[i].y + pstPosition->y;
        x = stTetris.astTetris[i].x + pstPosition->x;

        if (DATA_MAP_COL <= x || 0 > x || -4 > y || DATA_MAP_ROW <= y
                || (0 <= y && SHOW_SHARP_INVALID != piMap[DATA_INDEX_MAP(y, x)]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetris(&stTetris);

    return ME_MAX;
}

MAP_EVENT_E map_ActionCntrClk()
{
    int x, y;
    int *piMap;
    POINT_S *pstPosition;
    DATA_TETRIS_S stTetris;

    memcpy(&stTetris, DATA_GetCurrTetris(), sizeof(stTetris));
    for (int i = 0; i < ARRAY_SIZE(stTetris.astTetris); i++)
    {
        y = stTetris.astTetris[i].y;
        x = stTetris.astTetris[i].x;

        stTetris.astTetris[i].y = 3 - x;
        stTetris.astTetris[i].x = y;
    }

    pstPosition = DATA_GetCurrTetrisPos();
    piMap = DATA_GetMapCanvas();
    for (int i = 0; i < ARRAY_SIZE(stTetris.astTetris); i++)
    {
        y = stTetris.astTetris[i].y + pstPosition->y;
        x = stTetris.astTetris[i].x + pstPosition->x;

        if (DATA_MAP_COL <= x || 0 > x || -4 > y || DATA_MAP_ROW <= y
                || (0 <= y && SHOW_SHARP_INVALID != piMap[DATA_INDEX_MAP(y, x)]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetris(&stTetris);

    return ME_MAX;
}

MAP_EVENT_E map_ActionLand()
{
    int x, y, iCan, iIsOver = 0;
    int *piMap;
    POINT_S *pstPosition;
    DATA_TETRIS_S *pstTetris;
    DATA_RMROW_S stRmRow;

    piMap = DATA_GetMapCanvas();
    pstTetris = DATA_GetCurrTetris();
    pstPosition = DATA_GetCurrTetrisPos();

    for (int i = 0; i < ARRAY_SIZE(pstTetris->astTetris); i++)
    {
        y = pstTetris->astTetris[i].y + pstPosition->y;
        x = pstTetris->astTetris[i].x + pstPosition->x;

        if (0 <= y)
        {
            piMap[DATA_INDEX_MAP(y, x)] = pstTetris->iTetrisPixel;
        }

        if (0 >= y)
        {
            iIsOver = 1;
        }
    }

    memset(&stRmRow, 0, sizeof(stRmRow));
    for (int y = DATA_MAP_ROW - 1; 0 <= y; y--)
    {
        iCan = 1;
        for (int x = 0; DATA_MAP_COL > x; x++)
        {
            if (SHOW_SHARP_INVALID == piMap[DATA_INDEX_MAP(y, x)])
            {
                iCan = 0;
                break;
            }
        }

        if (iCan)
        {
            assert(stRmRow.iCount < ARRAY_SIZE(stRmRow.aiRow));
            stRmRow.aiRow[stRmRow.iCount] = y;
            stRmRow.iCount++;
        }
    }
    if (stRmRow.iCount)
    {
        DATA_SetRmRow(&stRmRow);
        DATA_SetCurrState(MS_REMOVE);
        return ME_MAX;
    }

    if (iIsOver)
    {
        DATA_SetCurrState(MS_OVER);
        return ME_MAX;
    }

    map_SetCurrTetris(DATA_GetNextTetris());
    DATA_UpdateNextTetris();
    DATA_SetCurrState(MS_WAITING);

    return ME_MAX;
}

MAP_EVENT_E map_ActionRemove(void)
{
    int iRemove, iIsOver;
    DATA_RMROW_S *pstRmRow;

    assert(MS_REMOVE == DATA_GetCurrState());

    if (MAIN_TIMECOUNT_REMOVE > DATA_UpdateTimeCount())
    {
        return ME_MAX;
    }

    DATA_ResetTimeCount();

    iRemove = map_TryRemoveCol();
    if (iRemove)
    {
        return ME_MAX;
    }

    pstRmRow = DATA_GetRmRow();
    DATA_IncreaseScore(pstRmRow->iCount);

    iIsOver = map_RemoveRow();
    if (iIsOver)
    {
        DATA_SetCurrState(MS_OVER);
    }
    else
    {
        map_SetCurrTetris(DATA_GetNextTetris());
        DATA_UpdateNextTetris();
        DATA_SetCurrState(MS_WAITING);
    }

    return ME_MAX;
}

MAP_EVENT_E map_TranWait()
{
    DATA_SetCurrState(MS_WAITING);

    return ME_MAX;
}

MAP_EVENT_E map_TranFast()
{
    DATA_SetCurrState(MS_FAST);

    return ME_MAX;
}

MAP_EVENT_E map_TranPause()
{
    DATA_SetCurrState(MS_PAUSE);

    return ME_MAX;
}

static MAP_PROCEVENT_PF g_apfMapFSM[MS_MAX][ME_MAX] =
{
        [MS_INVALID] =
        {
                [ME_INIT]       = map_ActionInit,
                [ME_START]      = NULL,
                [ME_TICK]       = NULL,
                [ME_LEFT]       = NULL,
                [ME_RIGHT]      = NULL,
                [ME_CLOCK]      = NULL,
                [ME_CNTRCLK]    = NULL,
                [ME_LAND]       = NULL,
                [ME_PAUSETOG]   = NULL,
                [ME_FASTTOG]    = NULL,
        },
        [MS_INIT] =
        {
                [ME_INIT]       = NULL,
                [ME_START]      = map_ActionStart,
                [ME_TICK]       = NULL,
                [ME_LEFT]       = NULL,
                [ME_RIGHT]      = NULL,
                [ME_CLOCK]      = NULL,
                [ME_CNTRCLK]    = NULL,
                [ME_LAND]       = NULL,
                [ME_PAUSETOG]   = NULL,
                [ME_FASTTOG]    = NULL,
        },
        [MS_WAITING] =
        {
                [ME_INIT]       = NULL,
                [ME_START]      = NULL,
                [ME_TICK]       = map_ActionDown,
                [ME_LEFT]       = map_ActionLeft,
                [ME_RIGHT]      = map_ActionRight,
                [ME_CLOCK]      = map_ActionClock,
                [ME_CNTRCLK]    = map_ActionCntrClk,
                [ME_LAND]       = map_ActionLand,
                [ME_PAUSETOG]   = map_TranPause,
                [ME_FASTTOG]    = map_TranFast,
        },
        [MS_FAST] =
        {
                [ME_INIT]       = NULL,
                [ME_START]      = NULL,
                [ME_TICK]       = map_ActionDown,
                [ME_LEFT]       = NULL,
                [ME_RIGHT]      = NULL,
                [ME_CLOCK]      = NULL,
                [ME_CNTRCLK]    = NULL,
                [ME_LAND]       = map_ActionLand,
                [ME_PAUSETOG]   = map_TranPause,
                [ME_FASTTOG]    = map_TranWait,
        },
        [MS_REMOVE] =
        {
                [ME_INIT]       = NULL,
                [ME_START]      = NULL,
                [ME_TICK]       = map_ActionRemove,
                [ME_LEFT]       = NULL,
                [ME_RIGHT]      = NULL,
                [ME_CLOCK]      = NULL,
                [ME_CNTRCLK]    = NULL,
                [ME_LAND]       = NULL,
                [ME_PAUSETOG]   = NULL,
                [ME_FASTTOG]    = NULL,
        },
        [MS_PAUSE] =
        {
                [ME_INIT]       = NULL,
                [ME_START]      = NULL,
                [ME_TICK]       = NULL,
                [ME_LEFT]       = NULL,
                [ME_RIGHT]      = NULL,
                [ME_CLOCK]      = NULL,
                [ME_CNTRCLK]    = NULL,
                [ME_LAND]       = NULL,
                [ME_PAUSETOG]   = map_TranWait,
                [ME_FASTTOG]    = NULL,
        },
        [MS_OVER] =
        {
                [ME_INIT]       = NULL,
                [ME_START]      = map_ActionInit,
                [ME_TICK]       = NULL,
                [ME_LEFT]       = NULL,
                [ME_RIGHT]      = NULL,
                [ME_CLOCK]      = NULL,
                [ME_CNTRCLK]    = NULL,
                [ME_LAND]       = NULL,
                [ME_PAUSETOG]   = NULL,
                [ME_FASTTOG]    = NULL,
        },
};

int map_FSM(MAP_EVENT_E enEvent)
{
    MAP_PROCEVENT_PF pfTrans;
    MAP_EVENT_E enTmpEvent;
    MAP_STATE_E enOldState;

    enOldState = DATA_GetCurrState();

    enTmpEvent = enEvent;
    while (ME_MAX != enTmpEvent)
    {
        enOldState = DATA_GetCurrState();
        pfTrans = g_apfMapFSM[enOldState][enTmpEvent];
        if (NULL != pfTrans)
        {
            enTmpEvent = pfTrans();
        }
        else
        {
            enTmpEvent = ME_MAX;
        }

        LOG_DEBUG("MAP-FSM: old-state=%d, event=%d, new-state=%d.", enOldState,
                enEvent, DATA_GetCurrState());
    }

    map_Show();

    return ERROR_SUCCESS;
}

int map_ProcEventTime()
{
    LOG_DEBUG("Time Input.");

    map_FSM(ME_TICK);

    return ERROR_SUCCESS;
}

int map_ProcEventKey(int iChar)
{
    MAP_EVENT_E enEvent;

    LOG_DEBUG("Key(%d) Input.", iChar);

    switch (iChar)
    {
    case 'a':
    case 'A':
        enEvent = ME_LEFT;
        break;
    case 'd':
    case 'D':
        enEvent = ME_RIGHT;
        break;
    case 'j':
    case 'J':
        enEvent = ME_CNTRCLK;
        break;
    case 'k':
    case 'K':
        enEvent = ME_CLOCK;
        break;
    case 's':
    case 'S':
        enEvent = ME_FASTTOG;
        break;
    case 'h':
    case 'H':
        enEvent = ME_PAUSETOG;
        break;
    case 'g':
    case 'G':
        enEvent = ME_START;
        break;
    default:
        enEvent = ME_MAX;
        break;
    }

    map_FSM(enEvent);

    return ERROR_SUCCESS;
}

int MAP_ProcEvent(MAIN_EVENT_E enEvent, void *pData)
{
    switch (enEvent)
    {
    case MAIN_EVENT_TICK:
    {
        map_ProcEventTime();
        break;
    }
    case MAIN_EVENT_KEY:
    {
        map_ProcEventKey((int) pData);
        break;
    }
    case MAIN_EVENT_EXIT:
    default:
    {
        assert(0);
        break;
    }
    }

    return ERROR_SUCCESS;
}

int MAP_Init()
{
    DATA_SetCurrState(MS_INVALID);
    map_FSM(ME_INIT);

    return ERROR_SUCCESS;
}

void MAP_Exit()
{
    return;
}
