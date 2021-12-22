#include "mcl/link/link_node_allocator.h"
#include "mcl/link/link_node.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE MclLinkNode* MclLinkNodeAllocator_AllocDefault(MclLinkNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclLinkNode));
}

MCL_PRIVATE void MclLinkNodeAllocator_ReleaseDefault(MclLinkNodeAllocator *self, MclLinkNode *node) {
    MCL_FREE(node);
}

MclLinkNodeAllocator* MclLinkNodeAllocator_GetDefault() {
    static MclLinkNodeAllocator defaultLinkNodeAllocator = {
            .alloc = MclLinkNodeAllocator_AllocDefault,
            .release = MclLinkNodeAllocator_ReleaseDefault
    };
    return &defaultLinkNodeAllocator;
}
