#include "mcl/map/hash_bucket.h"
#include "mcl/assert.h"

MCL_PRIVATE MclStatus MclHashBucket_RemoveNodeFromBucket(MclHashBucket* self,
		MclHashNode *node, MclHashNodeAllocator *allocator, MclHashValueDestroy destroy) {
	MCL_ASSERT_TRUE(MCL_LINK_NODE_IS_IN_LINK(node, link));
	MCL_LINK_REMOVE(node, link);
	MclHashNode_Delete(node, allocator, destroy);
	return MCL_SUCCESS;
}

void MclHashBucket_Init(MclHashBucket *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_LINK_INIT(&self->nodes, MclHashNode, link);
}

void MclHashBucket_Clear(MclHashBucket *self, MclHashNodeAllocator *allocator, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashNode *node, *tmpNode;
	MCL_LINK_FOREACH_SAFE(&self->nodes, MclHashNode, link, node, tmpNode) {
        (void)MclHashBucket_RemoveNodeFromBucket(self, node, allocator, destroy);
	}
}

bool MclHashBucket_IsEmpty(const MclHashBucket *self) {
	return self ? MCL_LINK_EMPTY(&self->nodes, MclHashNode, link) : true;
}

MclHashNode* MclHashBucket_FindNode(const MclHashBucket *self, MclHashKey key) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclHashNode *node;
	MCL_LINK_FOREACH(&self->nodes, MclHashNode, link, node) {
		if (node->key == key) {
			return node;
		}
	}
	return NULL;
}

MclStatus MclHashBucket_PushBackNode(MclHashBucket *self, MclHashNode *node) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(node);
	MCL_ASSERT_TRUE(!MclHashBucket_FindNode(self, node->key));

	MCL_LINK_INSERT_TAIL(&self->nodes, node, MclHashNode, link);
	return MCL_SUCCESS;
}

MclStatus MclHashBucket_RemoveNode(MclHashBucket *self, MclHashNode *node, MclHashNodeAllocator *allocator, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(node);

	MCL_ASSERT_SUCC_CALL(MclHashBucket_RemoveNodeFromBucket(self, node, allocator, destroy));
	return MCL_SUCCESS;
}

uint32_t MclHashBucket_RemoveBy(MclHashBucket *self, MclHashNodePred pred, void *arg, MclHashNodeAllocator *allocator, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	uint32_t removedCount = 0;
	MclHashNode *node, *tmpNode;
	MCL_LINK_FOREACH_SAFE(&self->nodes, MclHashNode, link, node, tmpNode) {
		if (MclHashNode_Pred(node, pred, arg)) {
			if (!MCL_FAILED(MclHashBucket_RemoveNodeFromBucket(self, node, allocator, destroy))) {
			    removedCount++;
			}
		}
	}
	return removedCount;
}

MCL_PRIVATE bool MclHashBucket_PredKeyEqual(const MclHashNode *node, void *arg) {
	return  MclHashNode_GetKey(node) == *(MclHashKey*)arg;
}

uint32_t MclHashBucket_Remove(MclHashBucket *self, MclHashKey key, MclHashNodeAllocator *allocator, MclHashValueDestroy destroy) {
	return MclHashBucket_RemoveBy(self, MclHashBucket_PredKeyEqual, &key, allocator, destroy);
}

MclStatus MclHashBucket_Accept(const MclHashBucket *self, MclHashNodeVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclHashNode *node;
	MCL_LINK_FOREACH(&self->nodes, MclHashNode, link, node) {
		MclStatus ret = MclHashNode_Visit(node, visit, arg);
		if (MCL_DONE(ret)) return MCL_SUCCESS;
		if (MCL_FAILED(ret)) return ret;
	}
	return MCL_SUCCESS;
}

void MclHashBucket_Dump(const MclHashBucket *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashNode *node;
	MCL_LINK_FOREACH(&self->nodes, MclHashNode, link, node) {
		MCL_LOG("Node (%llu : %p) -> ", node->key, node->value);
	}
	MCL_LOG("NULL\n");
}
