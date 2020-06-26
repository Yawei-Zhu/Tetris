/*
 * key.h
 *
 *  Created on: 2020Äê6ÔÂ12ÈÕ
 *      Author: 10248
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KEY_H_
#define KEY_H_

int KEY_Init(void);
void KEY_Exit(void);

void KEY_SetRunning(int iRunning);
int KEY_GetRunning(void);

#endif /* KEY_H_ */

#ifdef __cplusplus
}
#endif
