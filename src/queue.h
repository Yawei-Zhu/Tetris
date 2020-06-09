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
typedef void (*QUE_FREE_PF)(void *pData);

int QUE_IsFull(void);
QUE_HANDLE QUE_Init(QUE_FREE_PF pfFree);
void QUE_Exit(QUE_HANDLE hQueue);
void QUE_Clear(QUE_HANDLE hQueue);
void* QUE_Read(QUE_HANDLE hQueue);
int QUE_Write(QUE_HANDLE hQueue, void *pData);

#endif /* QUEUE_H_ */
