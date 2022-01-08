#include "mcl/list/list_node.h"
#include "mcl/list/list_node_allocator.h"
#include "mcl/assert.h"

MclListNode* MclListNode_Create(MclListData data, MclListNodeAllocator *allocator) {
	MclListNode* self = MclListNodeAllocator_Alloc(allocator);
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclListNode_Init(self, data);
	return self;
}

void MclListNode_Delete(MclListNode *self, MclListNodeAllocator *allocator, MclListDataDestroyIntf *destroyIntf) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    MclListDataDestroyIntf_Destroy(destroyIntf, self->data);
    MclListNodeAllocator_Free(allocator, self);
}
