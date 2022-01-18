#ifndef H70ED089B_E972_46F4_AC7F_112B137B6326
#define H70ED089B_E972_46F4_AC7F_112B137B6326

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclDeferNode);

MCL_INTERFACE(MclDeferNodeAllocator) {
    MclDeferNode* (*alloc)(MclDeferNodeAllocator*);
    void (*free)(MclDeferNodeAllocator*, MclDeferNode*);
};

MCL_INLINE MclDeferNode* MclDeferNodeAllocator_Alloc(MclDeferNodeAllocator *self) {
    return (self && self->alloc) ? self->alloc(self) : NULL;
}

MCL_INLINE void MclDeferNodeAllocator_Free(MclDeferNodeAllocator *self, MclDeferNode *node) {
    if (self && self->free) self->free(self, node);
}

extern MclDeferNodeAllocator MCL_DEFER_NODE_ALLOCATOR_DEFAULT;

MclDeferNodeAllocator* MclDeferNodeAllocator_GetDefault();

/////////////////////////////////////////////////////////
#define MCL_DEFER_NODE_ALLOCATOR(ALLOC, FREE) {.alloc = ALLOC, .free = FREE}

MCL_STDC_END

#endif
