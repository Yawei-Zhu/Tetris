/*
 * main.h
 *
 *  Created on: 2019-02-23
 *      Author: Wind
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAIN_H_
#define MAIN_H_

#define MAIN_NAME_UPPER  "TETRIS"
#define MAIN_NAME_LOWER  "tetris"

typedef int (*rpint)[];  // only use on return type of function

#define MAIN_REFRESH_INTERVAL  60
#define MAIN_TIMECOUNT_MAX     16

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

#ifdef __cplusplus
}
#endif
