#include "mcl/map/hash_node_allocator.h"
#include "mcl/map/hash_node.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE MclHashNode* MclHashNodeAllocator_AllocDefault(MclHashNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclHashNode));
}

MCL_PRIVATE void MclHashNodeAllocator_FreeDefault(MclHashNodeAllocator *self, MclHashNode *node) {
    if (node) MCL_FREE(node);
}

MclHashNodeAllocator MCL_HASH_NODE_ALLOCATOR_DEFAULT = {
    .alloc = MclHashNodeAllocator_AllocDefault,
    .free = MclHashNodeAllocator_FreeDefault
};

MclHashNodeAllocator* MclHashNodeAllocator_GetDefault() {
    return &MCL_HASH_NODE_ALLOCATOR_DEFAULT;
}
