/*
 * main.h
 *
 *  Created on: 2019-02-23
 *      Author: Wind
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAIN_NAME_UPPER  "TETRIS"
#define MAIN_NAME_LOWER  "tetris"

typedef int (*rpint)[];  // only use on return type of function

typedef struct tagPoint
{
    int x;
    int y;
}POINT_S;

typedef struct tagRect
{
    int l;
    int t;
    int r;
    int b;
}RECT_S;

#define MAIN_REFRESH_INTERVAL  16
#define MAIN_TIMECOUNT_WAIT    60
#define MAIN_TIMECOUNT_FAST     4
#define MAIN_TIMECOUNT_REMOVE   3

typedef enum tagMainEvent
{
    MAIN_EVENT_INVALID,
    MAIN_EVENT_TICK,
    MAIN_EVENT_KEY,
    MAIN_EVENT_EXIT,
    MAIN_EVENT_MAX,
} MAIN_EVENT_E;

int MAIN_WriteEvent(MAIN_EVENT_E enEvent, void *pData);

#endif /* MAIN_H_ */
