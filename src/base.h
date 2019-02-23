/*
 * base.h
 *
 *  Created on: 2018-07-15
 *      Author: Wind
 */

#ifndef BASE_H_
#define BASE_H_

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;

#define OFFSET(TYPE, MEMBER) ((ulong)(&((TYPE *)0)->MEMBER))
#define CONTAINER(pMember, TYPE, MEMBER) ((TYPE *)((uchar *)(pMember) - OFFSET(TYPE, MEMBER)))

#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define BIT_SET(f, v)   ((f) |= (v))
#define BIT_RESET(f, v) ((f) &= ~(v))

#define BIT_MATCH(f, v) ((f) & (v) == (v))
#define BIT_TEST(f, v)  ((f) & (v) != 0)

#ifdef ERROR_SUCCESS
#undef ERROR_SUCCESS
#endif

typedef enum tagErrorCode
{
    ERROR_SUCCESS  = 0x00000000,
    ERROR_FAILED   = 0x30000001,
    ERROR_EXIT     = 0x10000001,
}ERROR_CODE_E;

#endif /* BASE_H_ */
