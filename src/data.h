/*
 * data.h
 *
 *  Created on: 2018-09-14
 *      Author: Wind
 */

#ifndef DATA_H_
#define DATA_H_

#define DATA_TETRIS_NUM 7

#define TLV_BUFFLEN_MAX 65535

#define MAP_ROW 20
#define MAP_COL 10

typedef struct tagDataRun
{
    int aaiMapCanvas[MAP_ROW][MAP_COL];
    int aaiNextTetris[4][2];
    int aaiCurrTetris[4][2];
    int aiCurrTetrisPosition[2]; /* зјБъ(x, y) */
    MAP_STATE_E enCurrState;
}DATA_RUN_S;

int  DATA_Init(void);
void DATA_Exit(void);

int  DATA_Save();

rpint DATA_GetRandTetris(void);

rpint DATA_GetMapCanvas(void);
int   DATA_SetMapCanvas(int ppiCanvas[MAP_ROW][MAP_COL]);

rpint DATA_GetNextTetris(void);
int  DATA_SetNextTetris(int ppiTetris[4][2]);

rpint DATA_GetCurrTetris(void);
int  DATA_SetCurrTetris(int ppiTetris[4][2]);

int *DATA_GetCurrTetrisPosition(void);
int  DATA_SetCurrTetrisPosition(int piPosition[2]);

MAP_STATE_E  DATA_GetCurrState(void);
int          DATA_SetCurrState(MAP_STATE_E enState);




#endif /* DATA_H_ */
