#ifndef H22383929_F96E_447A_9D78_780E12E969BC
#define H22383929_F96E_447A_9D78_780E12E969BC

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclHashNode);

MCL_TYPE(MclHashNodeAllocator) {
    MclHashNode* (*alloc)(MclHashNodeAllocator*);
    void (*release)(MclHashNodeAllocator*, MclHashNode*);
};

MCL_INLINE MclHashNode* MclHashNodeAllocator_Alloc(MclHashNodeAllocator *self) {
    return (self && self->alloc) ? self->alloc(self) : NULL;
}

MCL_INLINE void MclHashNodeAllocator_Release(MclHashNodeAllocator *self, MclHashNode *node) {
    if (self && self->release) self->release(self, node);
}

MclHashNodeAllocator* MclHashNodeAllocator_GetDefault();

#define MCL_HASH_NODE_ALLOCATOR(ALLOC, RELEASE) {.alloc = ALLOC, .release = RELEASE}

MCL_STDC_END

#endif
