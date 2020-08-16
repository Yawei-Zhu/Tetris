/*
 * log.c
 *
 *  Created on: 2019-01-26
 *      Author: Wind
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "base.h"
#include "log.h"

#define LOG_FILENAME_LEN        255
#define LOG_TIMESTRING_LEN      31
#define LOG_LOGSTRING_LEN       511

char g_acLogFileName[LOG_FILENAME_LEN + 1] =
{ 0 };
LOG_LEVEL_E g_enLogPrintLevel = LOG_LEVEL_NONE;

typedef enum tagLog_TimeString_Type
{
    LOG_TIMESTR_TYPE_INVALID,
    LOG_TIMESTR_TYPE_NAME,
    LOG_TIMESTR_TYPE_LOG,
    LOG_TIMESTR_TYPE_MAX,
} LOG_TIMESTR_TYPE_E;

int log_GetTimeString(LOG_TIMESTR_TYPE_E enType, uint uiTimeBuffLen,
        char *pcTimeBuff)
{
    int iRet;
    time_t stSecond;
    struct timeval stTime;
    struct tm *pstTime;
    char *apFormat[] =
            { "", "%04d%02d%02d%02d%02d%02d%03hd",
                    "%04d-%02d-%02d %02d:%02d:%02d.%03hd" };

    assert(NULL != pcTimeBuff);

    /* 获取系统时间 */
    memset(&stTime, 0, sizeof(stTime));
    gettimeofday(&stTime, NULL);
    stSecond = stTime.tv_sec;
    pstTime = localtime(&stSecond);

    iRet = snprintf(pcTimeBuff, uiTimeBuffLen, apFormat[enType],
            1900 + pstTime->tm_year, 1 + pstTime->tm_mon, pstTime->tm_mday,
            pstTime->tm_hour, pstTime->tm_min, pstTime->tm_sec,
            stTime.tv_usec / 1000);

    return iRet;
}

int log_GetFileName(uint uiNameBuffLen, char *pcNameBuff)
{
    int iRet;
    char szTimeStr[LOG_TIMESTRING_LEN + 1];

    assert(NULL != pcNameBuff);

    szTimeStr[0] = 0;
    (void) log_GetTimeString(LOG_TIMESTR_TYPE_NAME, sizeof(szTimeStr),
            szTimeStr);

    iRet = snprintf(pcNameBuff, uiNameBuffLen, "E:\\root\\var\\tetris.log"); //_%s.log", szTimeStr);

    return iRet;
}
uint log_NewLogFileName()
{
    g_acLogFileName[0] = 0;
    (void) log_GetFileName(sizeof(g_acLogFileName), g_acLogFileName);

    return ERROR_SUCCESS;
}

int log_IsNeedPrint(LOG_LEVEL_E enLevel)
{
    assert(LOG_LEVEL_NONE < enLevel && LOG_LEVEL_MAX > enLevel);

    return enLevel <= g_enLogPrintLevel;
}

void LOG_Print(LOG_LEVEL_E enLevel, char *szFormat, ...)
{
    FILE *pstLogFile;
    va_list pstArgs;
    char szTime[LOG_TIMESTRING_LEN + 1];
    char szLog[LOG_LOGSTRING_LEN + 1];
    char *apLevel[] =
    { "", "ERROR", "WARN", "INFO", "DEBUG" };

    if (!log_IsNeedPrint(enLevel))
    {
        return;
    }

    pstLogFile = fopen(g_acLogFileName, "a+");
    if (NULL == pstLogFile)
    {
        return;
    }
    (void) log_GetTimeString(LOG_TIMESTR_TYPE_LOG, sizeof(szTime), szTime);

    szLog[0] = 0;
    va_start(pstArgs, szFormat);
    vsnprintf(szLog, sizeof(szLog), szFormat, pstArgs);
    va_end(pstArgs);

    fprintf(pstLogFile, "%%%s [%5s]: %s\n", szTime, apLevel[enLevel], szLog);

    fclose(pstLogFile);

    return;
}

LOG_LEVEL_E LOG_GetLogLevel(void)
{
    return g_enLogPrintLevel;
}

void LOG_SetLogLevel(LOG_LEVEL_E enLevel)
{
    assert(LOG_LEVEL_MAX > enLevel);

    g_enLogPrintLevel = enLevel;

    return;
}

uint LOG_Init(void)
{
    uint uiErrCode;

    g_enLogPrintLevel = LOG_LEVEL_DEBUG;
    uiErrCode = log_NewLogFileName();

    return uiErrCode;
}

void LOG_Exit(void)
{
    g_acLogFileName[0] = 0;
    g_enLogPrintLevel = LOG_LEVEL_NONE;

    return;
}
