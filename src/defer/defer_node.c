#include "mcl/defer/defer_node.h"
#include "mcl/defer/defer_node_allocator.h"
#include "mcl/assert.h"

MclDeferNode* MclDeferNode_Create(MclDeferFn func, void *ctxt, MclDeferNodeAllocator *allocator) {
	MclDeferNode* self = MclDeferNodeAllocator_Alloc(allocator);
	MCL_ASSERT_VALID_PTR_NIL(self);

	self->func = func;
	self->ctxt = ctxt;
	return self;
}

void MclDeferNode_Delete(MclDeferNode *self, MclDeferNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);
    MclDeferNodeAllocator_Free(allocator, self);
}
