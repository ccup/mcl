#include "mcl/defer/defer_node_allocator.h"
#include "mcl/defer/defer_node.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE MclDeferNode* MclDeferNodeAllocator_AllocDefault(MclDeferNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclDeferNode));
}

MCL_PRIVATE void MclDeferNodeAllocator_FreeDefault(MclDeferNodeAllocator *self, MclDeferNode *node) {
    if (node) MCL_FREE(node);
}

MclDeferNodeAllocator MCL_DEFER_NODE_ALLOCATOR_DEFAULT = {
        .alloc = MclDeferNodeAllocator_AllocDefault,
        .free = MclDeferNodeAllocator_FreeDefault
};

MclDeferNodeAllocator* MclDeferNodeAllocator_GetDefault() {
    return &MCL_DEFER_NODE_ALLOCATOR_DEFAULT;
}
