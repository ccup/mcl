#include "mcl/link/link_node.h"
#include "mcl/link/link_node_allocator.h"
#include "mcl/assert.h"

MclLinkNode* MclLinkNode_Create(MclLinkData data, MclLinkNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_NIL(allocator);
	MCL_ASSERT_VALID_PTR_NIL(allocator->alloc);

	MclLinkNode* self = allocator->alloc();
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclLinkNode_Init(self, data);
	return self;
}

void MclLinkNode_Delete(MclLinkNode *self, MclLinkNodeAllocator *allocator, MclLinkDataDeleter dataDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    if (dataDeleter && self->data) dataDeleter(self->data);
    if (allocator && allocator->release) allocator->release(self);
}
