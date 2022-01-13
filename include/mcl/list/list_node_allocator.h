#ifndef H22383929_F96E_447A_9D78_780E12E969BC
#define H22383929_F96E_447A_9D78_780E12E969BC

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclListNode);

MCL_INTERFACE(MclListNodeAllocator) {
    MclListNode* (*alloc)(MclListNodeAllocator*);
    void (*free)(MclListNodeAllocator*, MclListNode*);
};

MCL_INLINE MclListNode* MclListNodeAllocator_Alloc(MclListNodeAllocator *self) {
    return (self && self->alloc) ? self->alloc(self) : NULL;
}

MCL_INLINE void MclListNodeAllocator_Free(MclListNodeAllocator *self, MclListNode *node) {
    if (self && self->free) self->free(self, node);
}

extern MclListNodeAllocator MclListNodeAllocator_Default;

/////////////////////////////////////////////////////////
#define MCL_LIST_NODE_ALLOCATOR(ALLOC, FREE) {.alloc = ALLOC, .free = FREE}

MCL_STDC_END

#endif
