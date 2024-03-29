#ifndef H22383929_F96E_447A_9D78_780E12E969BD
#define H22383929_F96E_447A_9D78_780E12E969BD

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclHashNode);

MCL_INTERFACE(MclHashNodeAllocator) {
    MclHashNode* (*alloc)(MclHashNodeAllocator*);
    void (*free)(MclHashNodeAllocator*, MclHashNode*);
};

MCL_INLINE MclHashNode* MclHashNodeAllocator_Alloc(MclHashNodeAllocator *self) {
    return (self && self->alloc) ? self->alloc(self) : NULL;
}

MCL_INLINE void MclHashNodeAllocator_Free(MclHashNodeAllocator *self, MclHashNode *node) {
    if (self && self->free) self->free(self, node);
}

extern MclHashNodeAllocator MclHashNodeAllocator_Default;

///////////////////////////////////////////////////////////
#define MCL_HASH_NODE_ALLOCATOR(ALLOC, FREE) {.alloc = ALLOC, .free = FREE}

MCL_STDC_END

#endif
