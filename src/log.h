/*
 * log.h
 *
 *  Created on: 2019年1月23日
 *      Author: Wind
 */

#ifndef LOG_H_
#define LOG_H_

typedef enum tagLogLevel
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_MAX,
}LOG_LEVEL_E;

uint LOG_Init(void);
void LOG_Exit(void);

void LOG_Print(LOG_LEVEL_E enLevel, char *szFormat, ...);

#define LOG_ERROR(szFormat, ...) \
    LOG_Print(LOG_LEVEL_ERROR, szFormat, ##__VA_ARGS__)
#define LOG_WARN(szFormat, ...) \
    LOG_Print(LOG_LEVEL_WARN, (szFormat), ##__VA_ARGS__)
#define LOG_INFO(szFormat, ...) \
    LOG_Print(LOG_LEVEL_INFO, szFormat, ##__VA_ARGS__)
#define LOG_DEBUG(szFormat, ...) \
    LOG_Print(LOG_LEVEL_DEBUG, szFormat, ##__VA_ARGS__)

#endif /* LOG_H_ */
