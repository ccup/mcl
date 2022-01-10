#include "mcl/defer/defer.h"
#include "mcl/assert.h"

void MclDefer_Init(MclDefer *self, MclDeferNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_LINK_INIT(&self->defers, MclDeferNode, link);
	self->allocator = allocator;
}

void MclDefer_Clear(MclDefer *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclDeferNode *node = NULL;
    MclDeferNode *tmpnode = NULL;
    MCL_LINK_FOREACH_SAFE(&self->defers, MclDeferNode, link, node, tmpnode) {
        MCL_LINK_REMOVE(node, link);
        MclDeferNode_Delete(node, self->allocator);
    }
}

MclStatus MclDefer_AddNode(MclDefer *self, MclDeferNode *defer) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(defer);

    MCL_LINK_INSERT_HEAD(&self->defers, defer, MclDeferNode, link);
    return MCL_SUCCESS;
}

MclDeferNode* MclDefer_AddFunc(MclDefer *self, MclDeferFn func, void *ctxt) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_VALID_PTR_NIL(func);

    MclDeferNode *defer = MclDeferNode_Create(func, ctxt, self->allocator);
    MCL_ASSERT_VALID_PTR_NIL(defer);

    MCL_LINK_INSERT_HEAD(&self->defers, defer, MclDeferNode, link);
    return defer;
}

void MclDefer_Exec(MclDefer *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclDeferNode *node = NULL;
    MclDeferNode *tmpnode = NULL;
    MCL_LINK_FOREACH_SAFE(&self->defers, MclDeferNode, link, node, tmpnode) {
        MclDeferNode_Exec(node);
        MCL_LINK_REMOVE(node, link);
        MclDeferNode_Delete(node, self->allocator);
    }
}
