/*
 * map.h
 *
 *  Created on: 2018-07-15
 *      Author: Wind
 */

#ifndef MAP_H_
#define MAP_H_

typedef enum tagMapState
{
    MS_INIT,
    MS_WAITING,
    MS_FAST,
    MS_REMOVE ,
    MS_PAUSE,
    MS_OVER,
    MS_MAX,
}MAP_STATE_E;

typedef enum tagMapEvent
{
    ME_INIT,
    ME_START,
    ME_TICK,
    ME_DOWN,
    ME_LEFT,
    ME_RIGHT,
    ME_CLOCK,
    ME_CNTRCLK,
    ME_LAND,

    ME_PAUSETOG,
    ME_FASTTOG,
    ME_MAX,
}MAP_EVENT_E;

typedef enum tagMapInput
{
    MI_INVALID,
    MI_TIME,
    MI_KEY,
    MI_MAX,
}MAP_INPUT_E;

int  MAP_Init(void);
void MAP_Exit(void);
int  MAP_ProcEvent(MAIN_EVENT_E enEvent, void *pData);

#endif /* MAP_H_ */
