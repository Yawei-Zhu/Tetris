/*
 * show.h
 *
 *  Created on: 2018年9月2日
 *      Author: Wind
 */

#ifndef SHOW_H_
#define SHOW_H_

#define SHOW_ROW 22
#define SHOW_COL 22


int SHOW_ShowMap(int aaiMap[MAP_ROW][MAP_COL]);
int SHOW_ShowNext(int aaiNext[4][4]);
int SHOW_ShowTip(char *szTip);

int  SHOW_Init();
void SHOW_Exit();

#endif /* SHOW_H_ */
