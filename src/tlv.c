/*
 * tlv.c
 *
 *  Created on: 2019-04-14
 *      Author: Wind
 */


#include <assert.h>
#include <string.h>
#include <winsock.h>

#include "base.h"
#include "tlv.h"

void tlv_bswap(TLV_TL_S *pstTL)
{
    assert(NULL != pstTL);

    pstTL->usType = htons(pstTL->usType);
    pstTL->usLength = htons(pstTL->usLength);

    return;
}

ushort STLV_Set(uchar *pucBuff, ushort uiBuffLen, ushort usType,
        ushort usDataLen, uchar *pucData)
{
    TLV_TL_S stTL;
    ushort usTotalLen = 0;

    assert(NULL != pucBuff);

    if (uiBuffLen < usDataLen + TLV_TL_LEN)
    {
        return 0;
    }

    memset(&stTL, 0, TLV_TL_LEN);
    stTL.usType = usType;
    stTL.usLength = usDataLen + TLV_TL_LEN;
    tlv_bswap(&stTL);

    memcpy(pucBuff, &stTL, TLV_TL_LEN);
    usTotalLen += TLV_TL_LEN;

    if (usDataLen)
    {
        assert(NULL != pucData);

        memcpy(pucBuff + usTotalLen, pucData, usDataLen);
        usTotalLen += usDataLen;
    }

    return usTotalLen;
}

ushort STLV_Get(uchar *pucBuff, ushort usBuffLen, ushort *pusType,
        ushort *pusDataLen, uchar **ppucData)
{
    TLV_TL_S stTL;
    ushort usDataLen = 0;

    assert(NULL != pucBuff);
    assert(NULL != pusType);
    assert(NULL != pusDataLen);
    assert(NULL != ppucData);

    if (usBuffLen < TLV_TL_LEN)
    {
        return 0;
    }

    memcpy(&stTL, pucBuff, TLV_TL_LEN);
    tlv_bswap(&stTL);
    if (usBuffLen < stTL.usLength)
    {
        return 0;
    }

    *pusType = (TLV_TYPE_E) (uint) stTL.usType;
    usDataLen = stTL.usLength - TLV_TL_LEN;
    if (usDataLen)
    {
        *ppucData = pucBuff + TLV_TL_LEN;
    }
    else
    {
        *ppucData = NULL;
    }
    *pusDataLen = usDataLen;

    return stTL.usLength;
}
