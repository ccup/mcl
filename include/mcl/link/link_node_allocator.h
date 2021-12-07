#ifndef MCL_LINK_NODE_ALLOCATOR_H
#define MCL_LINK_NODE_ALLOCATOR_H

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_FWD(MclLinkNode);

MCL_TYPE_DEF(MclLinkNodeAllocator) {
    MclLinkNode* (*alloc)();
    void (*release)(MclLinkNode*);
};

MclLinkNodeAllocator* MclLinkNodeAllocator_GetDefault();

MCL_STDC_END

#endif
