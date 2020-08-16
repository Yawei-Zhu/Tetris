/*
 * key.c
 *
 *  Created on: 2020Äê6ÔÂ12ÈÕ
 *      Author: 10248
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <conio.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

#include "base.h"
#include "main.h"
#include "key.h"

pthread_t g_stKeyThread;
int g_iKeyThreadValid = 0;

int g_iKeyThreadRunning = 1;
pthread_mutex_t g_stKeyLock = PTHREAD_MUTEX_INITIALIZER;


int key_IsValid(int iChar)
{
    int iValid;

    switch (iChar)
    {
    case 'a':
    case 'A':
    case 's':
    case 'S':
    case 'd':
    case 'D':
    case 'j':
    case 'J':
    case 'k':
    case 'K':
    case 'h':
    case 'H':
    case 'g':
    case 'G':
    case 3:
        iValid = 1;
        break;
    default:
        iValid = 0;
        break;
    }

    return iValid;
}


void *key_Task(void * pData)
{
    int iChar;
    struct timespec stTime = {0, 1000000U};

    while (KEY_GetRunning())
    {
        iChar = 0;
        while(kbhit())
        {
            iChar = getch();
        }

        if (key_IsValid(iChar))
        {
            if (3 == iChar)
            {
                KEY_SetRunning(0);
                break;
            }

            (void) MAIN_WriteEvent(MAIN_EVENT_KEY, (void *)(uint) iChar);
        }

        nanosleep(&stTime, NULL);
    }

    (void) MAIN_WriteEvent(MAIN_EVENT_EXIT, NULL);
    KEY_SetRunning(1);

    return NULL;
}

void KEY_SetRunning(int iRunning)
{
    pthread_mutex_lock(&g_stKeyLock);
    g_iKeyThreadRunning = iRunning;
    pthread_mutex_unlock(&g_stKeyLock);

    return;
}

int KEY_GetRunning(void)
{
    int iRunning;

    pthread_mutex_lock(&g_stKeyLock);
    iRunning = g_iKeyThreadRunning;
    pthread_mutex_unlock(&g_stKeyLock);

    return iRunning;
}

int KEY_Init(void)
{
    int iRet;

    if (g_iKeyThreadValid)
    {
        return 0;
    }

    iRet = pthread_create(&g_stKeyThread, NULL, key_Task, NULL);
    if (iRet != 0)
    {
        return ERROR_FAILED;
    }

    g_iKeyThreadValid = 1;

    return ERROR_SUCCESS;
}

void KEY_Exit(void)
{
    if (!g_iKeyThreadValid)
    {
        return;
    }

    pthread_join(g_stKeyThread, NULL);
    g_iKeyThreadValid = 0;

    return;
}

#ifdef __cplusplus
}
#endif
