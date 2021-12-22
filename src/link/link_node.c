#include "mcl/link/link_node.h"
#include "mcl/link/link_node_allocator.h"
#include "mcl/assert.h"

MclLinkNode* MclLinkNode_Create(MclLinkData data, MclLinkNodeAllocator *allocator) {
	MclLinkNode* self = MclLinkNodeAllocator_Alloc(allocator);
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclLinkNode_Init(self, data);
	return self;
}

void MclLinkNode_Delete(MclLinkNode *self, MclLinkNodeAllocator *allocator, MclLinkDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    MclLinkDataDeleter_Destroy(dataDeleter, self->data);
    MclLinkNodeAllocator_Release(allocator, self);
}
