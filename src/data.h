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
#define DATA_MAP_COL 12
#define DATA_MAP_RXC (DATA_MAP_ROW * DATA_MAP_COL)

#define DATA_TETRIS_POINT_NUM 4
#define DATA_TETRIS_ROW DATA_TETRIS_POINT_NUM
#define DATA_TETRIS_COL DATA_TETRIS_POINT_NUM
#define DATA_TETRIS_RXC (DATA_TETRIS_ROW * DATA_TETRIS_COL)


#define DATA_INDEX(col, y, x)   ((col) * (y) + (x))

#define DATA_INDEX_MAP(y, x)    DATA_INDEX(DATA_MAP_COL, y, x)
#define DATA_INDEX_NEXT(y, x)   DATA_INDEX(DATA_TETRIS_COL, y, x)


typedef struct tagDataTetris
{
    POINT_S astTetris[DATA_TETRIS_POINT_NUM];
    int iTetrisPixel;
}DATA_TETRIS_S;

typedef struct tagDataRemoveRow
{
    int aiRow[DATA_TETRIS_POINT_NUM];
    int iCount;
}DATA_RMROW_S;

typedef struct tagDataRun
{
    int aiMapCanvas[DATA_MAP_RXC];
    DATA_TETRIS_S stNextTetris;
    DATA_TETRIS_S stCurrTetris;
    POINT_S stCurrTetrisPos;
    MAP_STATE_E enCurrState;
    DATA_RMROW_S stRmRow;
    int iTimeCount;
    int iScore;
    int iLevel;
} DATA_RUN_S;

int DATA_Init(void);
void DATA_Exit(void);

int DATA_Save();

DATA_RUN_S *DATA_GetRun(void);

void DATA_SetRandSeed(uint uiSeed);
DATA_TETRIS_S *DATA_GetRandTetris(void);

int *DATA_GetMapCanvas(void);
int DATA_SetMapCanvas(int piMax[DATA_MAP_RXC]);

DATA_TETRIS_S *DATA_GetNextTetris(void);
DATA_TETRIS_S *DATA_UpdateNextTetris(void);

DATA_TETRIS_S *DATA_GetCurrTetris(void);
int DATA_SetCurrTetris(DATA_TETRIS_S *pstTetris);

POINT_S *DATA_GetCurrTetrisPos(void);
int DATA_SetCurrTetrisPos(POINT_S *pstPosition);

MAP_STATE_E DATA_GetCurrState(void);
int DATA_SetCurrState(MAP_STATE_E enState);

DATA_RMROW_S *DATA_GetRmRow(void);
int DATA_SetRmRow(DATA_RMROW_S *pstRmRow);

int DATA_ResetTimeCount();
int DATA_UpdateTimeCount();
int DATA_PeekTimeCount();

int DATA_ResetScore(void);
int DATA_IncreaseScore(int iRow);
int DATA_PeekScore(void);

#endif /* DATA_H_ */
