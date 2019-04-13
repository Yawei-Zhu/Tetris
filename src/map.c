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
#include "show.h"
#include "log.h"
#include "data.h"

typedef MAP_EVENT_E (*MAP_PROCEVENT_PF)(void);

#define MAP_TIMECOUNT_MAX 30

int  g_iMapTimeCount;

void map_SetCurrTetris(int aaiTetris[4][2])
{
    int aiPosition[2];

    (void) DATA_SetCurrTetris(aaiTetris);

    aiPosition[0] = -4;
    aiPosition[1] = MAP_COL / 2 - 2;
    (void) DATA_SetCurrTetrisPosition(aiPosition);

    return;
}

void map_UpdateNextTetris(void)
{
    int (*ppiTetris)[2];

    ppiTetris = DATA_GetRandTetris();
    (void) DATA_SetNextTetris(ppiTetris);
}

void map_Show(void)
{
    int aaiMap[MAP_ROW][MAP_COL], aaiNext[4][4];
    int x, y, *piPosition;
    int (*ppiTetris)[2];

    memcpy(aaiMap, DATA_GetMapCanvas(), sizeof(aaiMap));

    ppiTetris = DATA_GetCurrTetris();
    piPosition = DATA_GetCurrTetrisPosition();
    for (int i = 0; i < 4; i++)
    {
        y = ppiTetris[i][0] + piPosition[0];
        x = ppiTetris[i][1] + piPosition[1];
        if(0 <= y)
        {
            assert(0 <= y && y < MAP_ROW && 0 <= x && x < MAP_COL);
            aaiMap[y][x] = MP_TYPE_POINT;
        }
    }

    for(y = 0; y < MAP_ROW; y++)
    {
        for(x = 0; x < MAP_COL; x++)
        {
            aaiMap[y][x] = MAP_PIXEL2CHAR(aaiMap[y][x]);
        }
    }

    SHOW_ShowMap(aaiMap);

    ppiTetris = DATA_GetNextTetris();
    memset(aaiNext, MAP_PIXEL2CHAR(MP_TYPE_INVALID), sizeof(aaiNext));
    for (int i = 0; i < 4; i++)
    {
        y = ppiTetris[i][0];
        x = ppiTetris[i][1];
        aaiNext[y][x] = MAP_PIXEL2CHAR(MP_TYPE_POINT);
    }

    SHOW_ShowNext(aaiNext);

    return;
}

MAP_EVENT_E map_ActionInit()
{
    int aaiMap[MAP_ROW][MAP_COL];
    int (*ppiTetris)[2];

    memset(aaiMap, 0, sizeof(aaiMap));
    (void) DATA_SetMapCanvas(aaiMap);

    srand(time(0));

    ppiTetris = DATA_GetRandTetris();
    map_SetCurrTetris(ppiTetris);

    map_UpdateNextTetris();

    g_iMapTimeCount = 0;

    return ME_WAIT;
}

MAP_EVENT_E map_ActionDown()
{
    int aiPosition[2], x, y;
    int (*ppiMap)[MAP_COL];
    int (*ppiCurrTetris)[2];

    memcpy(aiPosition, DATA_GetCurrTetrisPosition(), sizeof(aiPosition));
    aiPosition[0]++;

    ppiCurrTetris = DATA_GetCurrTetris();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = ppiCurrTetris[i][0] + aiPosition[0];
        x = ppiCurrTetris[i][1] + aiPosition[1];

        if (MAP_ROW <= y || (0 <= y && MP_TYPE_INVALID != ppiMap[y][x]))
        {
            return ME_LANDING;
        }
    }

    DATA_SetCurrTetrisPosition(aiPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionLeft()
{
    int aiPosition[2], x, y;
    int (*ppiMap)[MAP_COL];
    int (*ppiCurrTetris)[2];

    memcpy(aiPosition, DATA_GetCurrTetrisPosition(), sizeof(aiPosition));
    aiPosition[1]--;

    ppiCurrTetris = DATA_GetCurrTetris();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = ppiCurrTetris[i][0] + aiPosition[0];
        x = ppiCurrTetris[i][1] + aiPosition[1];

        if (0 > x || (0 <= y && MP_TYPE_INVALID != ppiMap[y][x]))
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
    int (*ppiMap)[MAP_COL];
    int (*ppiCurrTetris)[2];

    memcpy(aiPosition, DATA_GetCurrTetrisPosition(), sizeof(aiPosition));
    aiPosition[1]++;

    ppiCurrTetris = DATA_GetCurrTetris();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = ppiCurrTetris[i][0] + aiPosition[0];
        x = ppiCurrTetris[i][1] + aiPosition[1];

        if (MAP_COL <= x || (0 <= y && MP_TYPE_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetrisPosition(aiPosition);

    return ME_MAX;
}

MAP_EVENT_E map_ActionClock()
{
    int aaiTemp[4][2], x, y, *piPosition;
    int (*ppiMap)[MAP_COL];

    memcpy(aaiTemp, DATA_GetCurrTetris(), sizeof(aaiTemp));
    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        aaiTemp[i][0] = x;
        aaiTemp[i][1] = 3 - y;
    }

    piPosition = DATA_GetCurrTetrisPosition();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0] + piPosition[0];
        x = aaiTemp[i][1] + piPosition[1];

        if (MAP_COL <= x || 0 > x || -4 > y || MAP_ROW <= y ||
                (0 <= y && MP_TYPE_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetris(aaiTemp);

    return ME_MAX;
}

MAP_EVENT_E map_ActionCntrClk()
{
    int aaiTemp[4][2], x, y, *piPosition;
    int (*ppiMap)[MAP_COL];

    memcpy(aaiTemp, DATA_GetCurrTetris(), sizeof(aaiTemp));
    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        aaiTemp[i][0] = 3 - x;
        aaiTemp[i][1] = y;
    }

    piPosition = DATA_GetCurrTetrisPosition();
    ppiMap = DATA_GetMapCanvas();
    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0] + piPosition[0];
        x = aaiTemp[i][1] + piPosition[1];

        if (MAP_COL <= x || 0 > x || -4 > y || MAP_ROW <= y ||
                (0 <= y && MP_TYPE_INVALID != ppiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    DATA_SetCurrTetris(aaiTemp);

    return ME_MAX;
}

MAP_EVENT_E map_ActionLanding()
{
    int x, y, *piPosition;
    int (*ppiMap)[MAP_COL];
    int (*ppiCurrTetris)[2];

    ppiMap = DATA_GetMapCanvas();
    ppiCurrTetris = DATA_GetCurrTetris();
    piPosition = DATA_GetCurrTetrisPosition();
    for (int i = 0; i < 4; i++)
    {
        y = ppiCurrTetris[i][0] + piPosition[0];
        x = ppiCurrTetris[i][1] + piPosition[1];

        if (0 <= y)
        {
            ppiMap[y][x] = MP_TYPE_POINT;
        }
        else
        {
            return ME_OVER;
        }
    }

    map_SetCurrTetris(DATA_GetNextTetris());
    map_UpdateNextTetris();

    return ME_ANNLT;
}

MAP_EVENT_E map_ActionAnnlt(void)
{
    int aaiMap[MAP_ROW][MAP_COL], iCan, iCount;
    int (*ppiMap)[MAP_COL];

    iCount = 0;
    memset(aaiMap, 0, sizeof(aaiMap));

    ppiMap = DATA_GetMapCanvas();
    for (int y = MAP_ROW - 1; 0 <= y; y--)
    {
        iCan = 1;
        for (int x = 0; MAP_COL > x; x++)
        {
            aaiMap[y + iCount][x] = ppiMap[y][x];
            if (MP_TYPE_INVALID == ppiMap[y][x])
            {
                iCan = 0;
            }
        }

        if (iCan)
        {
            iCount++;
        }
    }

    if (iCount)
    {
        DATA_SetMapCanvas(aaiMap);
    }

    return ME_WAIT;
}

MAP_EVENT_E map_TranWait()
{
    DATA_SetCurrState(MS_WAITING);

    return ME_MAX;
}

MAP_EVENT_E map_TranPause()
{
    DATA_SetCurrState(MS_PAUSE);

    return ME_MAX;
}

MAP_EVENT_E map_TranFast()
{
    DATA_SetCurrState(MS_FAST);

    return ME_MAX;
}

MAP_EVENT_E map_TranOver()
{
    DATA_SetCurrState(MS_OVER);

    return ME_MAX;
}

static MAP_PROCEVENT_PF g_apfMapFSM[MS_MAX][ME_MAX] = {
        /* MS_UNINIT */
        {
                map_ActionInit,       /* ME_INIT       */
                NULL,                 /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                NULL,                 /* ME_LANDING    */
                NULL,                 /* ME_ANNLT      */
                map_TranWait,         /* ME_WAIT       */
                NULL,                 /* ME_PAUSETOG   */
                NULL,                 /* ME_FASTTOG    */
                NULL,                 /* ME_OVER       */
        },
        /* MS_WAITING */
        {
                NULL,                 /* ME_INIT       */
                map_ActionDown,       /* ME_DOWN       */
                map_ActionLeft,       /* ME_LEFT       */
                map_ActionRight,      /* ME_RIGHT      */
                map_ActionClock,      /* ME_CLOCK      */
                map_ActionCntrClk,    /* ME_CNTRCLK    */
                map_ActionLanding,    /* ME_LANDING    */
                map_ActionAnnlt,      /* ME_ANNLT      */
                NULL,                 /* ME_WAIT       */
                map_TranPause,        /* ME_PAUSETOG   */
                map_TranFast,         /* ME_FASTTOG    */
                map_TranOver,         /* ME_OVER       */
        },
        /* MS_FAST */
        {
                NULL,                 /* ME_INIT       */
                map_ActionDown,       /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                map_ActionLanding,    /* ME_LANDING    */
                map_ActionAnnlt,      /* ME_ANNLT      */
                map_TranWait,         /* ME_WAIT       */
                NULL,                 /* ME_PAUSETOG   */
                map_TranWait,         /* ME_FASTTOG    */
                map_TranOver,         /* ME_OVER       */
        },
        /* MS_PAUSE */
        {
                NULL,                 /* ME_INIT       */
                NULL,                 /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                NULL,                 /* ME_LANDING    */
                NULL,                 /* ME_ANNLT      */
                NULL,                 /* ME_WAIT       */
                map_TranWait,         /* ME_PAUSETOG   */
                NULL,                 /* ME_FASTTOG    */
                NULL,                 /* ME_OVER       */
        },
        /* MS_OVER */
        {
                map_ActionInit,       /* ME_INIT       */
                NULL,                 /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                NULL,                 /* ME_LANDING    */
                NULL,                 /* ME_ANNLT      */
                map_TranWait,         /* ME_WAIT       */
                NULL,                 /* ME_PAUSE      */
                NULL,                 /* ME_FAST       */
                NULL,                 /* ME_OVER       */
        },
};

int map_FSM(MAP_EVENT_E enEvent)
{
    MAP_PROCEVENT_PF pfTrans;
    MAP_EVENT_E enTmpEvent;
    MAP_STATE_E enOldState;

    enOldState = DATA_GetCurrState();

    enTmpEvent = enEvent;
    while(ME_MAX != enTmpEvent)
    {
        pfTrans = g_apfMapFSM[enOldState][enTmpEvent];
        if (NULL != pfTrans)
        {
            enTmpEvent = pfTrans();
        }
        else
        {
            enTmpEvent = ME_MAX;
        }
    }

    LOG_DEBUG("MAP-FSM: old-state=%d, event=%d, new-state=%d.",
              enOldState, enEvent, DATA_GetCurrState());

    map_Show();

    return ERROR_SUCCESS;
}

int map_ProcEventTime()
{
    g_iMapTimeCount++;

    LOG_DEBUG("Time Input.");

    if (MAP_TIMECOUNT_MAX <= g_iMapTimeCount || MS_FAST == DATA_GetCurrState())
    {
        map_FSM(ME_DOWN);

        g_iMapTimeCount = 0;
    }

    return ERROR_SUCCESS;
}

int map_ProcEventKey(int iChar)
{
    MAP_EVENT_E enEvent;

    LOG_DEBUG("Key(%d) Input.", iChar);

    switch (iChar) {
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
            enEvent = ME_INIT;
            break;
        default:
            enEvent = ME_MAX;
            break;
    }

    map_FSM(enEvent);

    return ERROR_SUCCESS;
}

int MAP_Input(MAP_INPUT_E enInput, void *pData)
{
    switch(enInput)
    {
        case MI_TIME:
        {
            map_ProcEventTime();
            break;
        }
        case MI_KEY:
        {
            assert(NULL != pData);
            map_ProcEventKey(*(int *) pData);
            break;
        }
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
    DATA_SetCurrState(MS_UNINIT);
    map_FSM(ME_INIT);

    return ERROR_SUCCESS;
}

void MAP_Exit()
{
    return;
}
