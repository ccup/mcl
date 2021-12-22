#include "mcl/list/list_node_allocator.h"
#include "mcl/list/list_node.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE MclListNode* MclListNodeAllocator_AllocDefault(MclListNodeAllocator *self) {
    return MCL_MALLOC(sizeof(MclListNode));
}

MCL_PRIVATE void MclListNodeAllocator_ReleaseDefault(MclListNodeAllocator *self, MclListNode *node) {
    MCL_FREE(node);
}

MclListNodeAllocator* MclListNodeAllocator_GetDefault() {
    static MclListNodeAllocator defaultListNodeAllocator = {
            .alloc = MclListNodeAllocator_AllocDefault,
            .release = MclListNodeAllocator_ReleaseDefault
    };
    return &defaultListNodeAllocator;
}
