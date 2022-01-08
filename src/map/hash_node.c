#include "mcl/map/hash_node.h"
#include "mcl/map/hash_node_allocator.h"
#include "mcl/assert.h"

MclHashNode* MclHashNode_Create(MclHashKey key, MclHashValue value, MclHashNodeAllocator *allocator) {
	MclHashNode* self = MclHashNodeAllocator_Alloc(allocator);
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclHashNode_Init(self, key, value);
	return self;
}

void MclHashNode_Delete(MclHashNode *self, MclHashNodeAllocator *allocator, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    if (destroy) destroy(self->value);
    MclHashNodeAllocator_Release(allocator, self);
}
