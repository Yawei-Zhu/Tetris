#ifdef  __cplusplus
extern "C" {
#endif

#ifndef __LIST_H__
#define __LIST_H__
#include <stdlib.h>
#include <assert.h>
/*
 *                     +--------------+    +--------------+
 *                     |   data...    |    |   data...    |
 * +--------------+    +--------------+    +--------------+
 * |   stHead     | +->|   stNode     | +->|   stNode     |
 * | +----------+ | |  | +----------+ | |  | +----------+ |
 * | | pstFirst |-C-+  | | pstNext  |-C-+  | | pstNext  |-C----+
 * | +----------+ |    | +----------+ |    | +----------+ |   -+-
 * +--------------+    +--------------+    +--------------+
 *                     |   data...    |    |   data...    |
 *                     +--------------+    +--------------+
 */
typedef struct tagSL_Node
{
    struct tagSL_Node *pstNext;
} SL_NODE_S;

typedef struct tagSL_Head
{
    SL_NODE_S *pstFirst;
} SL_HEAD_S;

typedef void (*SL_FREE_PF)(SL_NODE_S *pstNode);
typedef int (*SL_COMPARE_PF)(SL_NODE_S *pstNode1, SL_NODE_S *pstNode2);

static inline void SL_Init(SL_HEAD_S *pstHead);
static inline void SL_InitNode(SL_NODE_S *pstNode);
static inline void SL_AddFirst(SL_HEAD_S *pstHead, SL_NODE_S *pstNode);
static inline void SL_AddAfter(SL_NODE_S *pstPrev, SL_NODE_S *pstNode);
static inline SL_NODE_S* SL_DelFirst(SL_HEAD_S *pstHead);
static inline SL_NODE_S* SL_DelAfter(SL_NODE_S *pstPrev);
static inline SL_NODE_S* SL_First(SL_HEAD_S *pstHead);
static inline SL_NODE_S* SL_Next(SL_NODE_S *pstPrev);
static inline int SL_IsEmpty(SL_HEAD_S *pstHead);
static inline void SL_FreeAll(SL_HEAD_S *pstHead, SL_FREE_PF pfFree);
static inline void SL_Sort(SL_HEAD_S *pstHead, SL_COMPARE_PF pfCmp);

#define SL_TryAddAfter(pstHead, pstPrev, pstNode) \
           ((pstPrev) == NULL ? \
            SL_AddFirst((pstHead), (pstNode)) : \
            SL_AddAfter((pstPrev), (pstNode)))

#define SL_TryDelAfter(pstHead, pstPrev) \
           ((pstPrev) == NULL ? SL_DelFirst(pstHead) : SL_DelAfter(pstPrev))

#define SL_TryNext(pstHead, pstPrev) \
           ((pstPrev) == NULL ? SL_First(pstHead) : SL_Next(pstPrev))

#define SL_FOREACH(pstHead, pstNode) \
        for ((pstNode) = SL_First(pstHead); \
             NULL != (pstNode); \
             (pstNode) = SL_Next(pstNode))

#define SL_FOREACH_PREV(pstHead, pstNode, pstPrev)  \
        for ((pstNode) = SL_First(pstHead), (pstPrev) = NULL; \
             NULL != (pstNode) || ((pstPrev) = NULL, 0); \
             ({typeof(pstNode) __pstNode = SL_TryNext((pstHead), (pstPrev)); \
               ((__pstNode) == (pstNode) && NULL != (pstNode) ? \
                ((pstPrev) = (pstNode), (pstNode) = SL_Next(pstPrev)) : \
                ((pstNode) = (__pstNode)));}))

#define SL_FOREACH_NEXT(pstHead, pstNode, pstNext)  \
        for ((pstNode) = SL_First(pstHead), (pstNext) = NULL; \
             NULL != (pstNode) && ((pstNext) = SL_Next(pstNode), 1); \
             (pstNode) = (pstNext))

static inline void SL_Init(SL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    pstHead->pstFirst = NULL;
}

static inline void SL_InitNode(SL_NODE_S *pstNode)
{
    assert(NULL != pstNode);

    pstNode->pstNext = NULL;
}

static inline void SL_AddFirst(SL_HEAD_S *pstHead, SL_NODE_S *pstNode)
{
    assert(NULL != pstHead);
    assert(NULL != pstNode);

    pstNode->pstNext = pstHead->pstFirst;
    pstHead->pstFirst = pstNode;
    return;
}

static inline void SL_AddAfter(SL_NODE_S *pstPrev, SL_NODE_S *pstNode)
{
    assert(NULL != pstPrev);
    assert(NULL != pstNode);

    pstNode->pstNext = pstPrev->pstNext;
    pstPrev->pstNext = pstNode;

    return;
}

static inline SL_NODE_S* SL_DelFirst(SL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    SL_NODE_S *pstFirst = pstHead->pstFirst;
    if (NULL != pstFirst)
    {
        pstHead->pstFirst = pstFirst->pstNext;
        pstFirst->pstNext = NULL;
    }

    return pstFirst;
}

static inline SL_NODE_S* SL_DelAfter(SL_NODE_S *pstPrev)
{
    SL_NODE_S *pstNode;

    assert(NULL != pstPrev);

    pstNode = pstPrev->pstNext;
    if (NULL != pstNode)
    {
        pstPrev->pstNext = pstNode->pstNext;
        pstNode->pstNext = NULL;
    }

    return pstNode;
}

static inline SL_NODE_S* SL_First(SL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    return pstHead->pstFirst;
}

static inline SL_NODE_S* SL_Next(SL_NODE_S *pstPrev)
{
    assert(NULL != pstPrev);

    return pstPrev->pstNext;
}

static inline int SL_IsEmpty(SL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    return NULL == pstHead->pstFirst;
}

static inline void SL_FreeAll(SL_HEAD_S *pstHead, SL_FREE_PF pfFree)
{
    SL_NODE_S *pstNode;
    SL_NODE_S *pstNext;

    assert(NULL != pstHead);
    assert(NULL != pfFree);

    SL_FOREACH_NEXT(pstHead, pstNode, pstNext)
    {
        pfFree(pstNode);
        pstNode = NULL;
    }
    return;
}

static inline void SL_Sort(SL_HEAD_S *pstHead, SL_COMPARE_PF pfCmp)
{
    SL_NODE_S *pstNode, *pstPrev;

    assert(NULL != pstHead);
    assert(NULL != pfCmp);

    SL_FOREACH_PREV(pstHead, pstNode, pstPrev)
    {
    }

    return;
}


/*
 *  +--------------------------------------------------------------------+
 *  |                           +--------------+       +--------------+  |
 *  |                           |   data...    |       |   data...    |  |
 *  |    +--------------+       +--------------+       +--------------+  |
 *  | +->|   stHead     |<-+ +->|   stNode     |<-+ +->|   stNode     |<-+
 *  | |  | +----------+ |  | |  | +----------+ |  | |  | +----------+ |
 *  +-C--C-| pstPrev  | |  +-C--C-| pstPrev  | |  +-C--C-| pstPrev  | |
 *    |  | +----------+ |    |  | +----------+ |    |  | +----------+ |
 *    |  | | pstNext  |-C----+  | | pstNext  |-C----+  | | pstNext  |-C--+
 *    |  | +----------+ |       | +----------+ |       | +----------+ |  |
 *    |  +--------------+       +--------------+       +--------------+  |
 *    |                         |   data...    |       |   data...    |  |
 *    |                         +--------------+       +--------------+  |
 *    +------------------------------------------------------------------+
 */
typedef struct tagDL_Node
{
    struct tagDL_Node *pstPrev;
    struct tagDL_Node *pstNext;
} DL_NODE_S;

typedef struct tagDL_Head
{
    DL_NODE_S stHead;
} DL_HEAD_S;

typedef void (*DL_FREE_PF)(DL_NODE_S *pstNode);
typedef int (*DL_COMPARE_PF)(DL_NODE_S *pstNode1, DL_NODE_S *pstNode2);

static inline void DL_Init(DL_HEAD_S *pstHead);
static inline void DL_InitNode(DL_NODE_S *pstNode);
static inline void DL_AddLast(DL_HEAD_S *pstHead, DL_NODE_S *pstNode);
static inline void DL_AddFirst(DL_HEAD_S *pstHead, DL_NODE_S *pstNode);
static inline void DL_AddAfter(DL_NODE_S *pstPrev, DL_NODE_S *pstNode);
static inline void DL_AddBefore(DL_NODE_S *pstNext, DL_NODE_S *pstNode);
static inline DL_NODE_S* DL_Del(DL_NODE_S *pstNode);
static inline DL_NODE_S* DL_DelFirst(DL_HEAD_S *pstHead);
static inline DL_NODE_S* DL_DelLast(DL_HEAD_S *pstHead);
static inline DL_NODE_S* DL_DelAfter(DL_NODE_S *pstPrev);
static inline DL_NODE_S* DL_DelBefore(DL_NODE_S *pstNext);
static inline DL_NODE_S* DL_First(DL_HEAD_S *pstHead);
static inline DL_NODE_S* DL_Last(DL_HEAD_S *pstHead);
static inline DL_NODE_S* DL_Next(DL_NODE_S *pstPrev);
static inline DL_NODE_S* DL_Prev(DL_NODE_S *pstNext);
static inline int  DL_IsEmpty(DL_HEAD_S *pstHead);
static inline int  DL_IsEnd(DL_HEAD_S *pstHead, DL_NODE_S *pstNode);
static inline void DL_FreeAll(DL_HEAD_S *pstHead, DL_FREE_PF pfFree);

#define DL_TryAddAfter(pstHead, pstPrev, pstNode) \
           (DL_IsEnd(pstHead, pstPrev) ? \
            DL_AddFirst((pstHead), (pstNode)) : \
            DL_AddAfter((pstPrev), (pstNode)))

#define DL_TryAddBefore(pstHead, pstNext, pstNode) \
           (DL_IsEnd(pstHead, pstNext) ? \
            DL_AddLast((pstHead), (pstNode)) : \
            DL_AddBefore((pstNext), (pstNode)))

#define DL_TryDel(pstHead, pstNode) \
           (DL_IsEnd(pstHead, pstNode) ? NULL : DL_Del(pstNode))

#define DL_TryDelAfter(pstHead, pstPrev) \
           (DL_IsEnd(pstHead, pstPrev) ? \
            DL_DelFirst(pstHead) : \
            DL_DelAfter(pstPrev))

#define DL_TryDelBefore(pstHead, pstNext) \
           (DL_IsEnd(pstHead, pstNext) ? \
            DL_DelLast(pstHead) : \
            DL_DelBefore(pstNext))

#define DL_TryNext(pstHead, pstPrev) \
           ((DL_IsEnd(pstHead, pstPrev) ? DL_First(pstHead) : DL_Next(pstPrev))

#define DL_TryPrev(pstHead, pstNext) \
           (DL_IsEnd(pstHead, pstNext) ? DL_Last(pstHead) : DL_Prev(pstNext))

#define DL_FOREACH(pstHead, pstNode) \
        for ((pstNode) = DL_First(pstHead); \
             !DL_IsEnd((pstHead), (pstNode)) || ((pstNode) = NULL, 0); \
             (pstNode) = DL_Next(pstNode))

#define DL_FOREACH_NEXT(pstHead, pstNode, pstNext) \
        for ((pstNode) = DL_First(pstHead); \
             (!DL_IsEnd((pstHead), (pstNode)) && \
              ((pstNext) = DL_Next(pstNode), 1)) || \
             ((pstNode) = (pstNext) = NULL, 0); \
             (pstNode) = (pstNext))

#define DL_FOREACH_PREV(pstHead, pstNode, pstPrev) \
        for ((pstNode) = DL_Last(pstHead); \
             (!DL_IsEnd((pstHead), (pstNode)) && \
              ((pstPrev) = DL_Prev(pstNode), 1)) || \
             ((pstNode) = (pstPrev) = NULL, 0); \
             (pstNode) = (pstPrev))

static inline void DL_Init(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    pstHead->stHead.pstPrev = &pstHead->stHead;
    pstHead->stHead.pstNext = &pstHead->stHead;
    return;
}

static inline void DL_InitNode(DL_NODE_S *pstNode)
{
    assert(NULL != pstNode);

    pstNode->pstPrev = NULL;
    pstNode->pstNext = NULL;
    return;
}

static inline void DL_AddLast(DL_HEAD_S *pstHead, DL_NODE_S *pstNode)
{
    assert(NULL != pstHead);
    assert(NULL != pstNode);

    DL_AddBefore(&pstHead->stHead, pstNode);
    return;
}

static inline void DL_AddFirst(DL_HEAD_S *pstHead, DL_NODE_S *pstNode)
{
    assert(NULL != pstHead);
    assert(NULL != pstNode);

    DL_AddAfter(&pstHead->stHead, pstNode);
    return;
}

static inline void DL_AddAfter(DL_NODE_S *pstPrev, DL_NODE_S *pstNode)
{
    assert(NULL != pstPrev);
    assert(NULL != pstNode);

    pstPrev->pstNext->pstPrev = pstNode;
    pstNode->pstNext = pstPrev->pstNext;
    pstPrev->pstNext = pstNode;
    pstNode->pstPrev = pstPrev;
    return;
}

static inline void DL_AddBefore(DL_NODE_S *pstNext, DL_NODE_S *pstNode)
{
    assert(NULL != pstNext);
    assert(NULL != pstNode);

    pstNext->pstPrev->pstNext = pstNode;
    pstNode->pstPrev = pstNext->pstPrev;
    pstNext->pstPrev = pstNode;
    pstNode->pstNext = pstNext;
    return;
}

static inline DL_NODE_S* DL_Del(DL_NODE_S *pstNode)
{
    assert(NULL != pstNode);

    DL_NODE_S *pstPrev = pstNode->pstPrev;
    DL_NODE_S *pstNext = pstNode->pstNext;

    assert(NULL != pstPrev);
    assert(NULL != pstNext);

    pstPrev->pstNext = pstNext;
    pstNext->pstPrev = pstPrev;

    DL_InitNode(pstNode);

    return pstNode;
}

static inline DL_NODE_S* DL_DelFirst(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    if (DL_IsEmpty(pstHead))
        return NULL;

    return DL_Del(pstHead->stHead.pstNext);
}

static inline DL_NODE_S* DL_DelLast(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    if (DL_IsEmpty(pstHead))
        return NULL;

    return DL_Del(pstHead->stHead.pstPrev);
}

static inline DL_NODE_S* DL_DelAfter(DL_NODE_S *pstPrev)
{
    assert(NULL != pstPrev);

    return DL_Del(pstPrev->pstNext);
}

static inline DL_NODE_S* DL_DelBefore(DL_NODE_S *pstNext)
{
    assert(NULL != pstNext);

    return DL_Del(pstNext->pstPrev);
}

static inline DL_NODE_S* DL_First(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    if (DL_IsEmpty(pstHead))
        return NULL;

    return pstHead->stHead.pstNext;
}

static inline DL_NODE_S* DL_Last(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    if (DL_IsEmpty(pstHead))
        return NULL;

    return pstHead->stHead.pstPrev;
}

static inline DL_NODE_S* DL_Next(DL_NODE_S *pstPrev)
{
    assert(NULL != pstPrev);

    return pstPrev->pstNext;
}

static inline DL_NODE_S* DL_Prev(DL_NODE_S *pstNext)
{
    assert(NULL != pstNext);

    return pstNext->pstPrev;
}

static inline int DL_IsEmpty(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);

    return DL_IsEnd(pstHead, pstHead->stHead.pstNext);
}

static inline int DL_IsEnd(DL_HEAD_S *pstHead, DL_NODE_S *pstNode)
{
    assert(NULL != pstHead);

    if (NULL == pstNode)
        return 1;

    return &pstHead->stHead == pstNode;
}

static inline void DL_FreeAll(DL_HEAD_S *pstHead, DL_FREE_PF pfFree)
{
    DL_NODE_S *pstNode;
    DL_NODE_S *pstNext;

    assert(NULL != pstHead);
    assert(NULL != pfFree);

    DL_FOREACH_NEXT(pstHead, pstNode, pstNext)
    {
        pfFree(pstNode);
        pstNode = NULL;
    }
    return;
}

static inline void DL_Sort(DL_HEAD_S *pstHead, DL_COMPARE_PF pfCmp)
{
    DL_NODE_S *pstFirst, *pstLast, *pstLeft, *pstRight, *pstNext, *pstTemp;
    DL_NODE_S *apstFirst[100];
    int iTop = -1, flag;

    assert(NULL != pstHead);
    assert(NULL != pfCmp);

    pstFirst = DL_First(pstHead);
    pstLast  = DL_Last(pstHead);
    while (1)
    {
        while (pstFirst != pstLast && pstFirst != DL_Next(pstLast))
        {
            pstLeft = pstFirst;
            pstRight = pstLast;
            flag = 1;
            while (pstLeft != pstRight)
            {
                if (pfCmp(pstLeft, pstRight) > 0)
                {
                    pstNext = DL_Next(pstRight);
                    DL_Del(pstRight);
                    DL_AddBefore(pstLeft, pstRight);
                    DL_Del(pstLeft);
                    DL_AddBefore(pstNext, pstLeft);
                    pstTemp = pstRight;
                    pstRight = pstLeft;
                    pstLeft = pstTemp;
                    if (pstFirst == pstRight)
                    {
                        pstFirst = pstLeft;
                    }
                    if (pstLast == pstLeft)
                    {
                        pstLast = pstRight;
                    }
                    flag = !flag;
                }

                if (flag)
                {
                    pstRight = DL_Prev(pstRight);
                }
                else
                {
                    pstLeft = DL_Next(pstLeft);
                }
            }

            assert(iTop < 99);
            apstFirst[++iTop] = pstFirst;
            pstFirst = DL_Next(pstRight);
        }

        if (iTop == -1)
        {
            break;
        }
        pstLast = DL_Prev(DL_Prev(pstFirst));
        pstFirst = apstFirst[iTop--];
    }

    return;
}

#endif /* __LIST_H__ */

#ifdef  __cplusplus
}
#endif
