#ifndef MCL_LINK_NODE_ALLOCATOR_H
#define MCL_LINK_NODE_ALLOCATOR_H

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_FWD(MclLinkNode);

MCL_TYPE_DEF(MclLinkNodeAllocator) {
    MclLinkNode* (*alloc)(MclLinkNodeAllocator*);
    void (*release)(MclLinkNodeAllocator*, MclLinkNode*);
};

MCL_INLINE MclLinkNode* MclLinkNodeAllocator_Alloc(MclLinkNodeAllocator *self) {
    return (self && self->alloc) ? self->alloc(self) : NULL;
}

MCL_INLINE void MclLinkNodeAllocator_Release(MclLinkNodeAllocator *self, MclLinkNode *node) {
    if (self && self->release) self->release(self, node);
}

MclLinkNodeAllocator* MclLinkNodeAllocator_GetDefault();

#define MCL_LINK_NODE_ALLOCATOR(ALLOC, RELEASE) {.alloc = ALLOC, .release = RELEASE}

MCL_STDC_END

#endif
