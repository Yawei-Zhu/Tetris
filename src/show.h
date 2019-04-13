/*
 * show.h
 *
 *  Created on: 2018年9月2日
 *      Author: Wind
 */

#ifndef SHOW_H_
#define SHOW_H_

#define SHOW_ROW 20
#define SHOW_COL 17

int  SHOW_Init();
void SHOW_Exit();

int SHOW_ShowMap(int aaiMap[MAP_ROW][MAP_COL]);
int SHOW_ShowNext(int aaiNext[4][4]);

#endif /* SHOW_H_ */
