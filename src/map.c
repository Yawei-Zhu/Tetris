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
    int aiPosition[2];

    (void) DATA_SetCurrTetris(pstTetris);

    aiPosition[0] = -4;
    aiPosition[1] = DATA_MAP_COL / 2 - 2;
    (void) DATA_SetCurrTetrisPosition(aiPosition);

    return;
}

void map_UpdateNextTetris(void)
{
    DATA_TETRIS_S stTetris;

    DATA_GetRandTetris(&stTetris);
    (void) DATA_SetNextTetris(&stTetris);
}

void map_Show(void)
{
    int aaiMap[DATA_MAP_ROW][DATA_MAP_COL], aaiNext[4][4];
    int x, y, *piPosition;
    DATA_TETRIS_S *pstTetris;

    memcpy(aaiMap, DATA_GetMapCanvas(), sizeof(aaiMap));

    pstTetris = DATA_GetCurrTetris();
    piPosition = DATA_GetCurrTetrisPosition();
    for (int i = 0; i < 4; i++)
    {
        y = pstTetris->aaiTetris[i][0] + piPosition[0];
        x = pstTetris->aaiTetris[i][1] + piPosition[1];
        if (0 <= y)
        {
            assert(0 <= y && y < DATA_MAP_ROW && 0 <= x && x < DATA_MAP_COL);
            aaiMap[y][x] = pstTetris->iTetrisPixel;
        }
    }

    SHOW_ShowMap(aaiMap);

    pstTetris = DATA_GetNextTetris();
    memset(aaiNext, 0, sizeof(aaiNext));
    for (int i = 0; i < 4; i++)
    {
        y = pstTetris->aaiTetris[i][0];
        x = pstTetris->aaiTetris[i][1];
        aaiNext[y][x] = pstTetris->iTetrisPixel;
    }

    SHOW_ShowNext(aaiNext);

    return;
}

int map_RemoveRowMap(int aaiMap[DATA_MAP_ROW][DATA_MAP_COL])
{
    int iRemove, iCount;
    int (*ppiMap)[DATA_MAP_COL];

    ppiMap = DATA_GetMapCanvas();
    iCount = 0;
    for (int y = DATA_MAP_ROW - 1; y >= 0; y--)
    {
        iRemove = 1;
        for (int x = 0; x < DATA_MAP_COL; x++)
        {
            aaiMap[y + iCount][x] = ppiMap[y][x];
            if (SHOW_SHARP_INVALID != ppiMap[y][x])
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

int map_RemoveRowCurrTetris(int aaiMap[DATA_MAP_ROW][DATA_MAP_COL])
{
    int iRemove, iCount, x, y, iCannot = 0;
    int *piRmRow, *piCurrPosition;
    DATA_TETRIS_S *pstTetris;
    DATA_RMROW_S *pstRmRow;

    pstRmRow = DATA_GetRmRow();
    piRmRow = pstRmRow->aaiRow;
    assert(0 < pstRmRow->iCount);

    pstTetris = DATA_GetCurrTetris();
    piCurrPosition = DATA_GetCurrTetrisPosition();
    for (int i = 0; i < 4; i++)
    {
        iRemove = 0;
        iCount = 0;
        y = pstTetris->aaiTetris[i][0] + piCurrPosition[0];
        x = pstTetris->aaiTetris[i][1] + piCurrPosition[1];
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
                aaiMap[y][x] = pstTetris->iTetrisPixel;
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
    int iCannot;
    int aaiMap[DATA_MAP_ROW][DATA_MAP_COL];

    memset(aaiMap, 0, sizeof(aaiMap));

    (void) map_RemoveRowMap(aaiMap);

    iCannot = map_RemoveRowCurrTetris(aaiMap);

    (void) DATA_SetMapCanvas(aaiMap);

    return iCannot;
}

int map_TryRemoveCol(void)
{
    int aaiMap[DATA_MAP_ROW][DATA_MAP_COL];
    int iRemove, iLeft, iRight;
    int *piRmRow;
    DATA_RMROW_S *pstRmRow;

    memcpy(aaiMap, DATA_GetMapCanvas(), sizeof(aaiMap));
    pstRmRow = DATA_GetRmRow();
    piRmRow = pstRmRow->aaiRow;
    assert(0 < pstRmRow->iCount);

    iRemove = 0;
    iLeft = (DATA_MAP_COL - 1) / 2;
    iRight = (DATA_MAP_COL) / 2;
    for (; iLeft >= 0; iLeft--, iRight++)
    {
        assert(DATA_MAP_COL > iRight);
        if (SHOW_SHARP_INVALID == aaiMap[piRmRow[0]][iLeft])
        {
            continue;
        }

        for (int i = 0; i < pstRmRow->iCount; i++)
        {
            assert(SHOW_SHARP_INVALID != aaiMap[piRmRow[i]][iLeft]);
            aaiMap[piRmRow[i]][iLeft] = SHOW_SHARP_INVALID;
            if (iRight != iLeft)
            {
                assert(SHOW_SHARP_INVALID != aaiMap[piRmRow[i]][iRight]);
                aaiMap[piRmRow[i]][iRight] = SHOW_SHARP_INVALID;
            }
        }
        iRemove = 1;
        break;
    }

    if (iRemove)
    {
        (void) DATA_SetMapCanvas(aaiMap);
    }

    return iRemove;
}

MAP_EVENT_E map_ActionInit()
{
    int aaiMap[DATA_MAP_ROW][DATA_MAP_COL];
    DATA_TETRIS_S stTetris;

    memset(aaiMap, 0, sizeof(aaiMap));
    (void) DATA_SetMapCanvas(aaiMap);
    (void) DATA_ResetTimeCount();

    memset(&stTetris, 0, sizeof(stTetris));
    map_SetCurrTetris(&stTetris);

    DATA_SetRandSeed(time(NULL));
    map_UpdateNextTetris();

    DATA_SetCurrState(MS_INIT);

    return ME_MAX;
}

MAP_EVENT_E map_ActionStart()
{
    map_SetCurrTetris(DATA_GetNextTetris());
    map_UpdateNextTetris();
    DATA_SetCurrState(MS_WAITING);

    return ME_MAX;
}

MAP_EVENT_E map_ActionTick()
{
    if (MAIN_TIMECOUNT_MAX <= DATA_UpdateTimeCount())
    {
        return ME_DOWN;
    }

    return ME_MAX;
}

MAP_EVENT_E map_ActionDown()
{
    int aiPosition[2], x, y;
    int (*ppiMap)[DATA_MAP_COL];
    DATA_TETRIS_S *pstTetris;

    memcpy(aiPosition, DATA_GetCurrTetrisPosition(), sizeof(aiPosition));
    aiPosition[0]++;

    pstTetris = DATA_GetCurrTetris();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = pstTetris->aaiTetris[i][0] + aiPosition[0];
        x = pstTetris->aaiTetris[i][1] + aiPosition[1];

        if (DATA_MAP_ROW <= y || (0 <= y && SHOW_SHARP_INVALID != ppiMap[y][x]))
        {
            return ME_LAND;
        }
    }

    DATA_SetCurrTetrisPosition(aiPosition);
    DATA_ResetTimeCount();

    return ME_MAX;
}

MAP_EVENT_E map_ActionLeft()
{
    int aiPosition[2], x, y;
    int (*ppiMap)[DATA_MAP_COL];
    DATA_TETRIS_S *pstTetris;

    memcpy(aiPosition, DATA_GetCurrTetrisPosition(), sizeof(aiPosition));
    aiPosition[1]--;

    pstTetris = DATA_GetCurrTetris();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = pstTetris->aaiTetris[i][0] + aiPosition[0];
        x = pstTetris->aaiTetris[i][1] + aiPosition[1];

        if (0 > x || (0 <= y && SHOW_SHARP_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetrisPosition(aiPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionRight()
{
    int aiPosition[2], x, y;
    int (*ppiMap)[DATA_MAP_COL];
    DATA_TETRIS_S *pstTetris;

    memcpy(aiPosition, DATA_GetCurrTetrisPosition(), sizeof(aiPosition));
    aiPosition[1]++;

    pstTetris = DATA_GetCurrTetris();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = pstTetris->aaiTetris[i][0] + aiPosition[0];
        x = pstTetris->aaiTetris[i][1] + aiPosition[1];

        if (DATA_MAP_COL <= x || (0 <= y && SHOW_SHARP_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetrisPosition(aiPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionClock()
{
    int x, y, *piPosition;
    int (*ppiMap)[DATA_MAP_COL];
    DATA_TETRIS_S stTetris;

    memcpy(&stTetris, DATA_GetCurrTetris(), sizeof(stTetris));
    for (int i = 0; i < 4; i++)
    {
        y = stTetris.aaiTetris[i][0];
        x = stTetris.aaiTetris[i][1];

        stTetris.aaiTetris[i][0] = x;
        stTetris.aaiTetris[i][1] = 3 - y;
    }

    piPosition = DATA_GetCurrTetrisPosition();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = stTetris.aaiTetris[i][0] + piPosition[0];
        x = stTetris.aaiTetris[i][1] + piPosition[1];

        if (DATA_MAP_COL <= x || 0 > x || -4 > y || DATA_MAP_ROW <= y
                || (0 <= y && SHOW_SHARP_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetris(&stTetris);

    return ME_MAX;
}

MAP_EVENT_E map_ActionCntrClk()
{
    int x, y, *piPosition;
    int (*ppiMap)[DATA_MAP_COL];
    DATA_TETRIS_S stTetris;

    memcpy(&stTetris, DATA_GetCurrTetris(), sizeof(stTetris));
    for (int i = 0; i < 4; i++)
    {
        y = stTetris.aaiTetris[i][0];
        x = stTetris.aaiTetris[i][1];

        stTetris.aaiTetris[i][0] = 3 - x;
        stTetris.aaiTetris[i][1] = y;
    }

    piPosition = DATA_GetCurrTetrisPosition();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = stTetris.aaiTetris[i][0] + piPosition[0];
        x = stTetris.aaiTetris[i][1] + piPosition[1];

        if (DATA_MAP_COL <= x || 0 > x || -4 > y || DATA_MAP_ROW <= y
                || (0 <= y && SHOW_SHARP_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetris(&stTetris);

    return ME_MAX;
}

MAP_EVENT_E map_ActionLand()
{
    int x, y, *piPosition, iCan, iIsOver = 0;
    int (*ppiMap)[DATA_MAP_COL];
    DATA_TETRIS_S *pstTetris;
    DATA_RMROW_S stRmRow;

    ppiMap = DATA_GetMapCanvas();
    pstTetris = DATA_GetCurrTetris();
    piPosition = DATA_GetCurrTetrisPosition();

    for (int i = 0; i < 4; i++)
    {
        y = pstTetris->aaiTetris[i][0] + piPosition[0];
        x = pstTetris->aaiTetris[i][1] + piPosition[1];

        if (0 <= y)
        {
            ppiMap[y][x] = pstTetris->iTetrisPixel;
        }
        else
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
            if (SHOW_SHARP_INVALID == ppiMap[y][x])
            {
                iCan = 0;
                break;
            }
        }

        if (iCan)
        {
            assert(stRmRow.iCount < 4);
            stRmRow.aaiRow[stRmRow.iCount] = y;
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
    map_UpdateNextTetris();
    DATA_SetCurrState(MS_WAITING);

    return ME_MAX;
}

MAP_EVENT_E map_ActionRemove(void)
{
    int iRemove, iCannot;

    iRemove = map_TryRemoveCol();
    if (iRemove)
    {
        return ME_MAX;
    }

    iCannot = map_RemoveRow();
    if (iCannot)
    {
        DATA_SetCurrState(MS_OVER);
    }
    else
    {
        map_SetCurrTetris(DATA_GetNextTetris());
        map_UpdateNextTetris();
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
        [MS_INIT] =
        {
                [ME_INIT]       = map_ActionInit,
                [ME_START]      = map_ActionStart,
                [ME_TICK]       = NULL,
                [ME_DOWN]       = NULL,
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
                [ME_TICK]       = map_ActionTick,
                [ME_DOWN]       = map_ActionDown,
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
                [ME_DOWN]       = NULL,
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
                [ME_DOWN]       = NULL,
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
                [ME_DOWN]       = NULL,
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
                [ME_DOWN]       = NULL,
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
    DATA_SetCurrState(MS_INIT);
    map_FSM(ME_INIT);

    return ERROR_SUCCESS;
}

void MAP_Exit()
{
    return;
}
