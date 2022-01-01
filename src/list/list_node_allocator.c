#include "mcl/list/list_node_allocator.h"
#include "mcl/list/list_node.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE MclListNode* MclListNodeAllocator_AllocDefault(MclListNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclListNode));
}

MCL_PRIVATE void MclListNodeAllocator_ReleaseDefault(MclListNodeAllocator *self, MclListNode *node) {
    if (node) MCL_FREE(node);
}

MclListNodeAllocator MCL_LIST_NODE_ALLOCATOR = {
        .alloc = MclListNodeAllocator_AllocDefault,
        .release = MclListNodeAllocator_ReleaseDefault
};

MclListNodeAllocator* MclListNodeAllocator_GetDefault() {
    return &MCL_LIST_NODE_ALLOCATOR;
}
