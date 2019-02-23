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

typedef MAP_EVENT_E (*MAP_TRANSSTATE_PF)(void);
#define MAP_PIXEL2CHAR(mp) (MP_TYPE_INVALID == (mp) ? '.' : '#')

#define BLOCK_NUM 7
#define MAP_TIMECOUNT_MAX 30

int g_aaiMap[MAP_ROW][MAP_COL];
int g_aaiMapNextTetr[4][2];
int g_aaiMapCurrTetr[4][2];
int g_aiMapCurrTetrPosition[2];
MAP_STATE_E g_enMapCurrState;
int  g_iMapTimeCount;

void map_SetBuff(int aaiBuff[4][2])
{
    int dx, dy;

    memcpy(g_aaiMapCurrTetr, aaiBuff, sizeof(g_aaiMapCurrTetr));

    dx = MAP_COL / 2 - 2;
    dy = -4;

    g_aiMapCurrTetrPosition[0] = dy;
    g_aiMapCurrTetrPosition[1] = dx;

    for(int i = 0; i < 4; i++)
    {
        g_aaiMapCurrTetr[i][0] += dy;
        g_aaiMapCurrTetr[i][1] += dx;
    }
}

void map_GetRandTetr(int aaiTetr[4][2])
{
    int aaaiAllTetr[BLOCK_NUM][4][2] =
    {
            {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
            {{0, 1}, {0, 2}, {1, 2}, {2, 2}},
            {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
            {{1, 1}, {1, 2}, {2, 1}, {2, 2}},
            {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
            {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
            {{1, 2}, {2, 1}, {2, 2}, {2, 3}},
    };

    assert(NULL != aaiTetr);

    memcpy(aaiTetr, aaaiAllTetr[rand() % BLOCK_NUM], sizeof(aaaiAllTetr[0]));
}

void map_UpdateNext(void)
{
    map_GetRandTetr(g_aaiMapNextTetr);
}

void _map_Show(void)
{
    int aaiMap[MAP_ROW][MAP_COL], aaiNext[4][4];
    int x, y;

    memcpy(aaiMap, g_aaiMap, sizeof(aaiMap));

    for (int i = 0; i < 4; i++)
    {
        y = g_aaiMapCurrTetr[i][0];
        x = g_aaiMapCurrTetr[i][1];
        if(0 <= y)
        {
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

    memset(aaiNext, MAP_PIXEL2CHAR(MP_TYPE_INVALID), sizeof(aaiNext));
    for (int i = 0; i < 4; i++)
    {
        y = g_aaiMapNextTetr[i][0];
        x = g_aaiMapNextTetr[i][1];
        aaiNext[y][x] = MAP_PIXEL2CHAR(MP_TYPE_POINT);
    }

    SHOW_ShowNext(aaiNext);
}

MAP_EVENT_E _map_ActionInit()
{
    memset(g_aaiMap, 0, sizeof(g_aaiMap));

    srand(time(0));

    map_GetRandTetr(g_aaiMapCurrTetr);
    map_UpdateNext();

    g_iMapTimeCount = 0;

    //g_aiPosition[0] = 0;

    return ME_WAIT;
}

MAP_EVENT_E _map_ActionDown()
{
    int aaiTemp[4][2], x, y;

    for (int i = 0; i < 4; i++)
    {
        aaiTemp[i][0] = g_aaiMapCurrTetr[i][0] + 1;
        aaiTemp[i][1] = g_aaiMapCurrTetr[i][1];
    }

    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        if (MAP_ROW <= y || (0 <= y && MP_TYPE_INVALID != g_aaiMap[y][x]))
        {
            return ME_LANDING;
        }
    }

    memcpy(g_aaiMapCurrTetr, aaiTemp, sizeof(g_aaiMapCurrTetr));
    g_aiMapCurrTetrPosition[0]++;

    return ME_MAX;
}

MAP_EVENT_E _map_ActionLeft()
{
    int aaiTemp[4][2], x, y;

    for (int i = 0; i < 4; i++)
    {
        aaiTemp[i][0] = g_aaiMapCurrTetr[i][0];
        aaiTemp[i][1] = g_aaiMapCurrTetr[i][1] - 1;
    }

    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        if (0 > x || (0 <= y && MP_TYPE_INVALID != g_aaiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    memcpy(g_aaiMapCurrTetr, aaiTemp, sizeof(g_aaiMapCurrTetr));
    g_aiMapCurrTetrPosition[1]--;

    return ME_MAX;
}

MAP_EVENT_E _map_ActionRight()
{
    int aaiTemp[4][2], x, y;

    for (int i = 0; i < 4; i++)
    {
        aaiTemp[i][0] = g_aaiMapCurrTetr[i][0];
        aaiTemp[i][1] = g_aaiMapCurrTetr[i][1] + 1;
    }

    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        if (MAP_COL <= x || (0 <= y && MP_TYPE_INVALID != g_aaiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    memcpy(g_aaiMapCurrTetr, aaiTemp, sizeof(g_aaiMapCurrTetr));
    g_aiMapCurrTetrPosition[1]++;

    return ME_MAX;
}

MAP_EVENT_E _map_ActionClock()
{
    int aaiTemp[4][2], x, y;

    for (int i = 0; i < 4; i++)
    {
        y = g_aaiMapCurrTetr[i][0] - g_aiMapCurrTetrPosition[0];
        x = g_aaiMapCurrTetr[i][1] - g_aiMapCurrTetrPosition[1];

        aaiTemp[i][0] = g_aiMapCurrTetrPosition[0] + x;
        aaiTemp[i][1] = g_aiMapCurrTetrPosition[1] + 3 - y;
    }

    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        if (MAP_COL <= x || 0 > x || -4 > y || MAP_ROW <= y ||
                (0 <= y && MP_TYPE_INVALID != g_aaiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    memcpy(g_aaiMapCurrTetr, aaiTemp, sizeof(g_aaiMapCurrTetr));

    return ME_MAX;
}

MAP_EVENT_E _map_ActionCntrClk()
{
    int aaiTemp[4][2], x, y;

    for (int i = 0; i < 4; i++)
    {
        y = g_aaiMapCurrTetr[i][0] - g_aiMapCurrTetrPosition[0];
        x = g_aaiMapCurrTetr[i][1] - g_aiMapCurrTetrPosition[1];

        aaiTemp[i][0] = g_aiMapCurrTetrPosition[0] + 3 - x;
        aaiTemp[i][1] = g_aiMapCurrTetrPosition[1] + y;
    }

    for (int i = 0; i < 4; i++)
    {
        y = aaiTemp[i][0];
        x = aaiTemp[i][1];

        if (MAP_COL <= x || 0 > x || -4 > y || MAP_ROW <= y ||
                (0 <= y && MP_TYPE_INVALID != g_aaiMap[y][x]))
        {
            return ME_MAX;
        }
    }

    memcpy(g_aaiMapCurrTetr, aaiTemp, sizeof(g_aaiMapCurrTetr));

    return ME_MAX;
}

MAP_EVENT_E _map_ActionLanding()
{
    int x, y;

    for (int i = 0; i < 4; i++)
    {
        y = g_aaiMapCurrTetr[i][0];
        x = g_aaiMapCurrTetr[i][1];

        if (0 <= y)
        {
            g_aaiMap[y][x] = MP_TYPE_POINT;
        }
        else
        {
            return ME_OVER;
        }
    }

    map_SetBuff(g_aaiMapNextTetr);
    map_UpdateNext();

    return ME_ANNLT;
}

MAP_EVENT_E _map_ActionAnnlt(void)
{
    int iCan, iCount;
    int aaiMap[MAP_ROW][MAP_COL];

    iCount = 0;
    memset(aaiMap, 0, sizeof(aaiMap));

    for (int y = MAP_ROW - 1; 0 <= y; y--)
    {
        iCan = 1;
        for (int x = 0; MAP_COL > x; x++)
        {
            aaiMap[y + iCount][x] = g_aaiMap[y][x];
            if (MP_TYPE_INVALID == g_aaiMap[y][x])
            {
                iCan = 0;
            }
        }

        if (iCan)
        {
            iCount++;
        }
    }

    if (!iCan)
    {
        memcpy(g_aaiMap, aaiMap, sizeof(g_aaiMap));
    }

    return ME_WAIT;
}

MAP_EVENT_E _map_TranWait()
{
    g_enMapCurrState = MS_WAITING;

    return ME_MAX;
}

MAP_EVENT_E _map_TranPause()
{
    g_enMapCurrState = MS_PAUSE;

    return ME_MAX;
}

MAP_EVENT_E _map_TranFast()
{
    g_enMapCurrState = MS_FAST;

    return ME_MAX;
}

MAP_EVENT_E _map_TranOver()
{
    g_enMapCurrState = MS_OVER;

    return ME_MAX;
}

static MAP_TRANSSTATE_PF g_apfMapFSM[MS_MAX][ME_MAX] = {
        /* MS_UNINIT */
        {
                _map_ActionInit,      /* ME_INIT       */
                NULL,                 /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                NULL,                 /* ME_LANDING    */
                NULL,                 /* ME_ANNLT      */
                _map_TranWait,        /* ME_WAIT       */
                NULL,                 /* ME_PAUSETOG   */
                NULL,                 /* ME_FASTTOG    */
                NULL,                 /* ME_OVER       */
        },
        /* MS_WAITING */
        {
                NULL,                 /* ME_INIT       */
                _map_ActionDown,      /* ME_DOWN       */
                _map_ActionLeft,      /* ME_LEFT       */
                _map_ActionRight,     /* ME_RIGHT      */
                _map_ActionClock,     /* ME_CLOCK      */
                _map_ActionCntrClk,   /* ME_CNTRCLK    */
                _map_ActionLanding,   /* ME_LANDING    */
                _map_ActionAnnlt,     /* ME_ANNLT      */
                NULL,                 /* ME_WAIT       */
                _map_TranPause,       /* ME_PAUSETOG   */
                _map_TranFast,        /* ME_FASTTOG    */
                _map_TranOver,        /* ME_OVER       */
        },
        /* MS_FAST */
        {
                NULL,                 /* ME_INIT       */
                _map_ActionDown,      /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                _map_ActionLanding,   /* ME_LANDING    */
                _map_ActionAnnlt,     /* ME_ANNLT      */
                _map_TranWait,        /* ME_WAIT       */
                NULL,                 /* ME_PAUSETOG   */
                _map_TranWait,        /* ME_FASTTOG    */
                _map_TranOver,        /* ME_OVER       */
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
                _map_TranWait,        /* ME_PAUSETOG   */
                NULL,                 /* ME_FASTTOG    */
                NULL,                 /* ME_OVER       */
        },
        /* MS_OVER */
        {
                _map_ActionInit,      /* ME_INIT       */
                NULL,                 /* ME_DOWN       */
                NULL,                 /* ME_LEFT       */
                NULL,                 /* ME_RIGHT      */
                NULL,                 /* ME_CLOCK      */
                NULL,                 /* ME_CNTRCLK    */
                NULL,                 /* ME_LANDING    */
                NULL,                 /* ME_ANNLT      */
                _map_TranWait,        /* ME_WAIT       */
                NULL,                 /* ME_PAUSE      */
                NULL,                 /* ME_FAST       */
                NULL,                 /* ME_OVER       */
        },
};

int _map_FSM(MAP_EVENT_E enEvent)
{
    MAP_TRANSSTATE_PF pfTrans;
    MAP_EVENT_E enTmpEvent = enEvent;

    LOG_DEBUG("Map FSM: current-state=%d, event=%d", g_enMapCurrState, enEvent);

    while(ME_MAX != enTmpEvent)
    {
        pfTrans = g_apfMapFSM[g_enMapCurrState][enTmpEvent];
        if (NULL != pfTrans)
        {
            enTmpEvent = pfTrans();
        }
        else
        {
            enTmpEvent = ME_MAX;
        }
    }

    LOG_DEBUG("Map FSM: next-state=%d", g_enMapCurrState);

    _map_Show();

    return ERROR_SUCCESS;
}

int _map_ProcEventTime()
{
    g_iMapTimeCount++;

    if (MAP_TIMECOUNT_MAX <= g_iMapTimeCount || MS_FAST == g_enMapCurrState)
    {
        g_iMapTimeCount = 0;

        _map_FSM(ME_DOWN);
    }

    return ERROR_SUCCESS;
}

int _map_ProcEventKey(int iChar)
{
    char szTip[20];
    MAP_EVENT_E enEvent;

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
            sprintf(szTip, "Error key(%c)! ", iChar);
            SHOW_ShowTip(szTip);
            enEvent = ME_MAX;
            break;
    }

    _map_FSM(enEvent);

    return ERROR_SUCCESS;
}
int MAP_Input(MAP_INPUT_E enInput, void *pData)
{
    LOG_DEBUG("Input type(%d)", enInput);

    switch(enInput)
    {
        case MI_TIME:
        {
            _map_ProcEventTime();
            break;
        }
        case MI_KEY:
        {
            assert(NULL != pData);
            _map_ProcEventKey(*(int *) pData);
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
    g_enMapCurrState = MS_UNINIT;

    _map_FSM(ME_INIT);

    return ERROR_SUCCESS;
}

void MAP_Exit()
{
    return;
}
