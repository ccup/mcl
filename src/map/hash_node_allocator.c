#include "mcl/map/hash_node_allocator.h"
#include "mcl/map/hash_node.h"
#include "mcl/mem/memory.h"

MCL_PRIVATE MclHashNode* MclHashNodeAllocator_AllocDefault(MclHashNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclHashNode));
}

MCL_PRIVATE void MclHashNodeAllocator_FreeDefault(MclHashNodeAllocator *self, MclHashNode *node) {
    if (node) MCL_FREE(node);
}

MclHashNodeAllocator MclHashNodeAllocator_Default = {
    .alloc = MclHashNodeAllocator_AllocDefault,
    .free = MclHashNodeAllocator_FreeDefault
};
