/*
 * byteswap.c
 *
 *  Created on: 2019-04-14
 *      Author: Wind
 */


#include <assert.h>
#include <stddef.h>
#include <winsock.h>

#include "base.h"
#include "tlv.h"

typedef void (*BSWAP_PROC_PF)(TLV_TYPE_E enType, ulong ulDataLen, void *pData);

BSWAP_PROC_PF g_apfBSwapProcFunc[TLV_TYPE_MAX] =
        {
                [TLV_TYPE_INVALID] = NULL,
        };

void BSWAP_Proc(TLV_TYPE_E enType, ulong ulDataLen, void *pData)
{
    assert(0    != ulDataLen);
    assert(NULL != pData);

    if (NULL != g_apfBSwapProcFunc[enType])
    {
        g_apfBSwapProcFunc[enType](enType, ulDataLen, pData);
    }
    else
    {
        assert(0);
    }

    return;
}
