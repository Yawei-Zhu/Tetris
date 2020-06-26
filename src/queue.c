/*
 * queue.c
 *
 *  Created on: 2020Äê6ÔÂ7ÈÕ
 *      Author: 10248
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <pthread.h>

#include "base.h"
#include "list.h"
#include "queue.h"

typedef enum tagQueueState
{
    QS_DEFUALT,
    QS_CLEARING,
    QS_TODELETE,
    QS_MAX,
} QUE_STATE_E;

typedef struct tagQueue
{
    pthread_mutex_t stMutex;
    pthread_cond_t stReadCond;
    QUE_FREE_PF pfFree;
    DL_HEAD_S stQueue;
    uint uiSize;
    uint uiProcCount;
    QUE_STATE_E enState;
} QUE_S;

typedef struct tagQueueNode
{
    DL_NODE_S stNode;
    uint uiMsgType;
    void *pData;
} QUE_NODE_S;

#define QUE_MAX_NUM 10UL

QUE_S *g_apstQueArray[QUE_MAX_NUM] =
        { 0 };
pthread_mutex_t g_stQueMutex = PTHREAD_MUTEX_INITIALIZER;

void que_FreeAll(QUE_S *pstQueue)
{
    DL_NODE_S *pstCurr;
    QUE_NODE_S *pstNode;

    pstQueue->uiProcCount++;
    pthread_mutex_unlock(&g_stQueMutex);

    pthread_mutex_lock(&pstQueue->stMutex);
    while ((pstCurr = DL_DelFirst(&pstQueue->stQueue)) != NULL)
    {
        pstQueue->uiSize--;
        pstNode = CONTAINER(pstCurr, QUE_NODE_S, stNode);
        if (pstQueue->pfFree != NULL)
        {
            pstQueue->pfFree(pstNode->uiMsgType, pstNode->pData);
        }
        free(pstNode);
        pstNode = NULL;
    }
    pthread_mutex_unlock(&pstQueue->stMutex);

    pthread_mutex_lock(&g_stQueMutex);
    pstQueue->uiProcCount--;

    return;
}

int QUE_IsFull(void)
{
    int i;
    int iIsFull = 1;

    pthread_mutex_lock(&g_stQueMutex);
    for (i = 0; i < QUE_MAX_NUM; i++)
    {
        if (g_apstQueArray[i] == NULL)
        {
            iIsFull = 0;
            break;
        }
    }
    pthread_mutex_unlock(&g_stQueMutex);

    return iIsFull;
}

QUE_HANDLE QUE_Init(QUE_FREE_PF pfFree)
{
    QUE_S *pstQueue;
    int i;

    pstQueue = (QUE_S*) malloc(sizeof(QUE_S));
    if (NULL == pstQueue)
    {
        return QUE_INVALID_HANDLE;
    }

    memset(pstQueue, 0, sizeof(QUE_S));
    DL_Init(&pstQueue->stQueue);
    pthread_mutex_init(&pstQueue->stMutex, NULL);
    pthread_cond_init(&pstQueue->stReadCond, NULL);
    pstQueue->enState = QS_DEFUALT;
    pstQueue->pfFree = pfFree;

    pthread_mutex_lock(&g_stQueMutex);
    for (i = 0; i < QUE_MAX_NUM; i++)
    {
        if (g_apstQueArray[i] == NULL)
        {
            g_apstQueArray[i] = pstQueue;
            break;
        }
    }
    pthread_mutex_unlock(&g_stQueMutex);

    if (i >= QUE_MAX_NUM)
    {
        free(pstQueue);
        return QUE_INVALID_HANDLE;
    }

    return (QUE_HANDLE) i;
}

void QUE_Exit(QUE_HANDLE hQueue)
{
    QUE_S *pstQueue;

    if (hQueue >= QUE_MAX_NUM)
    {
        return;
    }

    pthread_mutex_lock(&g_stQueMutex);
    pstQueue = g_apstQueArray[hQueue];
    if (pstQueue == NULL)
    {
        pthread_mutex_unlock(&g_stQueMutex);
        return;
    }

    if (pstQueue->uiProcCount == 0)
    {
        que_FreeAll(pstQueue);
        free(pstQueue);
        g_apstQueArray[(int) hQueue] = NULL;
    }
    else
    {
        pstQueue->enState = QS_TODELETE;
    }
    pthread_mutex_unlock(&g_stQueMutex);

    return;
}

int QUE_Read(QUE_HANDLE hQueue, QUE_MSG_S *pstMsg, unsigned int uiTime)
{
    QUE_S *pstQueue;
    QUE_NODE_S *pstNode = NULL;
    struct timespec stTime;
    int iRet = -1;

    if ((hQueue >= QUE_MAX_NUM) || (NULL == pstMsg))
    {
        return -1;
    }

    pthread_mutex_lock(&g_stQueMutex);
    pstQueue = g_apstQueArray[hQueue];
    if (pstQueue == NULL)
    {
        pthread_mutex_unlock(&g_stQueMutex);
        return -1;
    }

    if (pstQueue->enState == QS_TODELETE)
    {
        pthread_mutex_unlock(&g_stQueMutex);
        return -1;
    }
    pstQueue->uiProcCount++;
    pthread_mutex_unlock(&g_stQueMutex);

    pthread_mutex_lock(&pstQueue->stMutex);
    if (DL_IsEmpty(&pstQueue->stQueue))
    {
        if (uiTime != QUE_TIME_NOTWAIT)
        {
            if (uiTime == QUE_TIME_WAITFOREVER)
            {
                pthread_cond_wait(&pstQueue->stReadCond, &pstQueue->stMutex);
            }
            else if (uiTime > QUE_TIME_NOTWAIT)
            {
                stTime.tv_sec = uiTime / 1000;
                stTime.tv_nsec = uiTime % 1000 * 1000000;
                pthread_cond_timedwait(&pstQueue->stReadCond,
                        &pstQueue->stMutex,
                        &stTime);
            }
            pthread_mutex_unlock(&pstQueue->stMutex);

            pthread_mutex_lock(&g_stQueMutex);
            if (pstQueue->enState == QS_TODELETE)
            {
                pstQueue->uiProcCount--;
                if (pstQueue->uiProcCount == 0)
                {
                    que_FreeAll(pstQueue);
                    free(pstQueue);
                    g_apstQueArray[(int) hQueue] = NULL;
                }
                pthread_mutex_unlock(&g_stQueMutex);
                return -1;
            }
            pthread_mutex_unlock(&g_stQueMutex);

            pthread_mutex_lock(&pstQueue->stMutex);
        }
    }

    if (!DL_IsEmpty(&pstQueue->stQueue))
    {
        pstNode = CONTAINER(DL_DelFirst(&pstQueue->stQueue), QUE_NODE_S,
                stNode);
        pstQueue->uiSize--;
    }
    pthread_mutex_unlock(&pstQueue->stMutex);

    if (NULL != pstNode)
    {
        pstMsg->uiMsgType = pstNode->uiMsgType;
        pstMsg->pData = pstNode->pData;
        free(pstNode);
        iRet = 0;
    }

    pthread_mutex_lock(&g_stQueMutex);
    pstQueue->uiProcCount--;
    if ((pstQueue->enState == QS_TODELETE) && (pstQueue->uiProcCount == 0))
    {
        que_FreeAll(pstQueue);
        free(pstQueue);
        g_apstQueArray[(int) hQueue] = NULL;
    }
    pthread_mutex_unlock(&g_stQueMutex);

    return iRet;
}

int QUE_Write(QUE_HANDLE hQueue, QUE_MSG_S *pstMsg)
{
    QUE_S *pstQueue;
    QUE_NODE_S *pstNode;

    if ((hQueue >= QUE_MAX_NUM) || (NULL == pstMsg))
    {
        return -1;
    }

    pthread_mutex_lock(&g_stQueMutex);
    pstQueue = g_apstQueArray[hQueue];
    if (pstQueue == NULL)
    {
        pthread_mutex_unlock(&g_stQueMutex);
        return -1;
    }

    if (pstQueue->enState == QS_TODELETE)
    {
        pthread_mutex_unlock(&g_stQueMutex);
        return -1;
    }
    pstQueue->uiProcCount++;
    pthread_mutex_unlock(&g_stQueMutex);

    pstNode = (QUE_NODE_S*) malloc(sizeof(QUE_NODE_S));
    if (NULL != pstNode)
    {
        DL_InitNode(&pstNode->stNode);
        pstNode->uiMsgType = pstMsg->uiMsgType;
        pstNode->pData = pstMsg->pData;

        pthread_mutex_lock(&pstQueue->stMutex);
        DL_AddLast(&pstQueue->stQueue, &pstNode->stNode);
        pstQueue->uiSize++;
        pthread_cond_signal(&pstQueue->stReadCond);
        pthread_mutex_unlock(&pstQueue->stMutex);
    }

    pthread_mutex_lock(&g_stQueMutex);
    pstQueue->uiProcCount--;
    if ((pstQueue->enState == QS_TODELETE) && (pstQueue->uiProcCount == 0))
    {
        que_FreeAll(pstQueue);
        free(pstQueue);
        g_apstQueArray[(int) hQueue] = NULL;
    }
    pthread_mutex_unlock(&g_stQueMutex);

    return 0;

}

#ifdef __cplusplus
}
#endif
