#include "mcl/link/link_node_allocator.h"
#include "mcl/link/link_node.h"
#include "mcl/mem/malloc.h"

MclLinkNode* MclLinkNodeAllocator_AllocDefault() {
    return MCL_MALLOC(sizeof(MclLinkNode));
}

void MclLinkNodeAllocator_ReleaseDefault(MclLinkNode *node) {
    MCL_FREE(node);
}

MclLinkNodeAllocator* MclLinkNodeAllocator_GetDefault() {
    static MclLinkNodeAllocator defaultLinkNodeAllocator = {
            .alloc = MclLinkNodeAllocator_AllocDefault,
            .release = MclLinkNodeAllocator_ReleaseDefault
    };
    return &defaultLinkNodeAllocator;
}
