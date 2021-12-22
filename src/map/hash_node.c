#include "mcl/map/hash_node.h"
#include "mcl/map/hash_node_allocator.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclHashNode_Init(MclHashNode *self, MclHashKey key, MclHashValue value) {
    self->link.prev = NULL;
    self->link.next = NULL;
    self->key = key;
    self->value = value;
}

MclHashNode* MclHashNode_Create(MclHashKey key, MclHashValue value, MclHashNodeAllocator *allocator) {
	MclHashNode* self = MclHashNodeAllocator_Alloc(allocator);
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclHashNode_Init(self, key, value);
	return self;
}

void MclHashNode_Delete(MclHashNode *self, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    MclHashValueDeleter_Destroy(valueDeleter, self->value);
    MclHashNodeAllocator_Release(allocator, self);
}
