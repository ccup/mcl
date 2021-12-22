#include "mcl/list/list_node.h"
#include "mcl/list/list_node_allocator.h"
#include "mcl/assert.h"

MclListNode* MclListNode_Create(MclListData data, MclListNodeAllocator *allocator) {
	MclListNode* self = MclListNodeAllocator_Alloc(allocator);
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclListNode_Init(self, data);
	return self;
}

void MclListNode_Delete(MclListNode *self, MclListNodeAllocator *allocator, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    MclListDataDeleter_Destroy(dataDeleter, self->data);
    MclListNodeAllocator_Release(allocator, self);
}
