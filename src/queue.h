/*
 * queue.h
 *
 *  Created on: 2020Äê6ÔÂ7ÈÕ
 *      Author: 10248
 */

#ifndef QUEUE_H_
#define QUEUE_H_

typedef unsigned int QUE_HANDLE;
#define QUE_INVALID_HANDLE ((QUE_HANDLE) -1)
typedef void (*QUE_FREE_PF)(unsigned int uiMsgType, void *pData);

typedef struct tagQueueMsg
{
    unsigned int uiMsgType;
    void *pData;
} QUE_MSG_S;

#define QUE_TIME_NOTWAIT 0U
#define QUE_TIME_WAITFOREVER ((unsigned int)(int) -1)

int QUE_IsFull(void);

QUE_HANDLE QUE_Init(QUE_FREE_PF pfFree);
void QUE_Exit(QUE_HANDLE hQueue);

int QUE_Read(QUE_HANDLE hQueue, QUE_MSG_S *pstMsg, unsigned int uiTimeout);
int QUE_Write(QUE_HANDLE hQueue, QUE_MSG_S *pstMsg);

#endif /* QUEUE_H_ */
