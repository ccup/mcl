#include "mcl/map/hash_node_allocator.h"
#include "mcl/map/hash_node.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE MclHashNode* MclHashNodeAllocator_AllocDefault(MclHashNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclHashNode));
}

MCL_PRIVATE void MclHashNodeAllocator_ReleaseDefault(MclHashNodeAllocator *self, MclHashNode *node) {
    MCL_FREE(node);
}

MclHashNodeAllocator* MclHashNodeAllocator_GetDefault() {
    static MclHashNodeAllocator defaultAllocator = {
            .alloc = MclHashNodeAllocator_AllocDefault,
            .release = MclHashNodeAllocator_ReleaseDefault
    };
    return &defaultAllocator;
}
