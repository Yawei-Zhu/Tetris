/*
 * show.h
 *
 *  Created on: 2018-9-2
 *      Author: Wind
 */

#ifndef SHOW_H_
#define SHOW_H_

#define SHOW_SHARP_INVALID   0x00
#define SHOW_SHARP_POINT     0x01
#define SHOW_SHARP_S         0x40
#define SHOW_SHARP_H         0x41
#define SHOW_SHARP_V         0x42
#define SHOW_SHARP_LT        0x43
#define SHOW_SHARP_LB        0x44
#define SHOW_SHARP_RT        0x45
#define SHOW_SHARP_RB        0x46

#define SHOW_SHARP_MASK      0xFF

#define SHOW_COLOR_HY        0x06
#define SHOW_COLOR_WHITE     0x07
#define SHOW_COLOR_BLUE      0x09
#define SHOW_COLOR_GREEN     0x0A
#define SHOW_COLOR_LG        0x0B
#define SHOW_COLOR_RED       0x0C
#define SHOW_COLOR_PURPLE    0x0D
#define SHOW_COLOR_YELLOW    0x0E

#define SHOW_COLOR_DEFAULT   SHOW_COLOR_WHITE
#define SHOW_COLOR_MASK      0xFF

#define SHOW_PIXEL(color, sharp) ((((color) & SHOW_COLOR_MASK) << 16) | \
                                  (((sharp) & SHOW_SHARP_MASK) << 00))

#define SHOW_SHARP(pixel) (((pixel) >> 00) & SHOW_SHARP_MASK)
#define SHOW_COLOR(pixel) (((pixel) >> 16) & SHOW_COLOR_MASK)

#define SHOW_POINT_PIXEL(color)  SHOW_PIXEL((color), SHOW_SHARP_POINT)

#define SHOW_TABS_PIXEL(sharp)   SHOW_PIXEL(SHOW_COLOR_DEFAULT, (sharp))

#define SHOW_INVALID_PIXEL   SHOW_PIXEL(SHOW_COLOR_DEFAULT, SHOW_SHARP_INVALID)

int  SHOW_Init();
void SHOW_Exit();

int SHOW_ShowMap(int aiMap[DATA_MAP_RXC]);
int SHOW_ShowNext(int aiNext[DATA_TETRIS_RXC]);
int SHOW_ShowScore(int iScore);

#endif /* SHOW_H_ */
