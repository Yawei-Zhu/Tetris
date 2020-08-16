/*
 * tlv.h
 *
 *  Created on: 2019-04-14
 *      Author: Wind
 */

#ifndef TLV_H_
#define TLV_H_

typedef enum tagTlvType
{
    TLV_TYPE_INVALID = 0,
    TLV_TYPE_CANVAS,
    TLV_TYPE_CURR_TITRIS,
    TLV_TYPE_NEXT_TITRIS,
    TLV_TYPE_CURR_POSITION,
    TLV_TYPE_CURR_STATE,
    TLV_TYPE_MAX
} TLV_TYPE_E;

typedef struct tagTlvTypeLength
{
    ushort usType;
    ushort usLength;
} TLV_TL_S;

#define TLV_TL_LEN  sizeof(TLV_TL_S)

ushort STLV_Set(uchar *pucBuff, ushort uiBuffLen, ushort usType,
        ushort usDataLen, uchar *pucData);

ushort STLV_Get(uchar *pucBuff, ushort uiBuffLen, ushort *pusType,
        ushort *pusDataLen, uchar **ppucData);

#define STLV_FOREACH(pucBuff, usBuffLen, usType, usDataLen, pucData) \
    for(STLV_Get((pucBuff), (usBuffLen), &(usType), &(usDataLen), &(pucData)); \
        NULL != (pucData); \
        (void) STLV_Get(((char *) (pucData) + (usDataLen)),  \
                       (ushort) (((ulong)(pucBuff) + (usBuffLen)) - \
                                 ((ulong)(pucData)  + (usDataLen))), \
                       &(usType), &(usDataLen), &(pucData)))

/* 8×Ö½Ú¶ÔÆë */
#define TLV_OCTET_ALIGN(x)  ((x + 7UL) / 8UL * 8UL)

#define TLV_BUFF_FLAG_NONE       0
#define TLV_BUFF_FLAG_BALLOC     0x0001
#define TLV_BUFF_FLAG_TALLOC     0x0002

struct tagTlvBuff;
typedef struct tagTlvBuff TLV_BUFF_S;
typedef uint (*TLV_BUFF_FLUSH_PF)(TLV_BUFF_S *pstTlvBuff);

struct tagTlvBuff
{
    uchar *pucBuff;
    ushort usBuffLen;
    ushort usCurrLen;
    ushort usOffset;
    ushort usFlags;
    ulong aulPara[4];
    TLV_BUFF_FLUSH_PF pfFlush;
};

TLV_BUFF_S* TLV_Init(uchar *pucBuff, ushort usBuffLen);
void TLV_Exit(TLV_BUFF_S *pstTlvBuff);

void TLV_Append(TLV_BUFF_S *pstTlvBuff, ushort usType, ushort usDataLen,
        uchar *pucData);

#endif /* TLV_H_ */
