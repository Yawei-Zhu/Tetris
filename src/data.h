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

#define DATA_MAP_ROW 20
#define DATA_MAP_COL 10

#define DATA_NEXT_ROW 4
#define DATA_NEXT_COL 4

typedef struct tagDataTetris
{
    int aaiTetris[4][2];
    int iTetrisPixel;
}DATA_TETRIS_S;

typedef struct tagDataRemoveRow
{
    int aaiRow[4];
    int iCount;
}DATA_RMROW_S;

typedef struct tagDataRun
{
    int aaiMapCanvas[DATA_MAP_ROW][DATA_MAP_COL];
    DATA_TETRIS_S stNextTetris;
    DATA_TETRIS_S stCurrTetris;
    int aiCurrTetrisPosition[2]; /* зјБъ(x, y) */
    MAP_STATE_E enCurrState;
    DATA_RMROW_S stRmRow;
    int iTimeCount;
} DATA_RUN_S;

int DATA_Init(void);
void DATA_Exit(void);

int DATA_Save();

void DATA_SetRandSeed(uint uiSeed);
void DATA_GetRandTetris(DATA_TETRIS_S *pstTetris);

rpint DATA_GetMapCanvas(void);
int DATA_SetMapCanvas(int ppiCanvas[DATA_MAP_ROW][DATA_MAP_COL]);

DATA_TETRIS_S *DATA_GetNextTetris(void);
int DATA_SetNextTetris(DATA_TETRIS_S *pstTetris);

DATA_TETRIS_S *DATA_GetCurrTetris(void);
int DATA_SetCurrTetris(DATA_TETRIS_S *pstTetris);

int* DATA_GetCurrTetrisPosition(void);
int DATA_SetCurrTetrisPosition(int piPosition[2]);

MAP_STATE_E DATA_GetCurrState(void);
int DATA_SetCurrState(MAP_STATE_E enState);

DATA_RMROW_S *DATA_GetRmRow(void);
int DATA_SetRmRow(DATA_RMROW_S *pstRmRow);

int DATA_ResetTimeCount();
int DATA_UpdateTimeCount();
int DATA_PeekTimeCount();

#endif /* DATA_H_ */
