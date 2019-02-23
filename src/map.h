/*
 * map.h
 *
 *  Created on: 2018-07-15
 *      Author: Wind
 */

#ifndef MAP_H_
#define MAP_H_

#define MAP_ROW 20
#define MAP_COL 10

typedef enum tagMapPixel_Type{
	MP_TYPE_INVALID = 0,
	MP_TYPE_POINT,
}MP_TYPE_E;

typedef enum tagMapState
{
    MS_UNINIT,
    MS_WAITING,
    MS_FAST,
    MS_PAUSE,
    MS_OVER,
    MS_MAX,
}MAP_STATE_E;

typedef enum tagMapEvent
{
    ME_INIT,
    ME_DOWN,
    ME_LEFT,
    ME_RIGHT,
    ME_CLOCK,
    ME_CNTRCLK,
    ME_LANDING,
    ME_ANNLT,

    ME_WAIT,
    ME_PAUSETOG,
    ME_FASTTOG,
    ME_OVER,
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
int  MAP_Input(MAP_INPUT_E enInput, void *pData);

#endif /* MAP_H_ */
