#include "mcl/map/hash_bucket.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclHashBucket_RemoveNode(MclHashBucket *self,
		MclHashNode *node, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_LINK_REMOVE(node, link);
	MclHashNode_Delete(node, allocator, valueDeleter);
	self->count--;
}

void MclHashBucket_Init(MclHashBucket *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MCL_LINK_INIT(&self->nodes, MclHashNode, link);
	self->count = 0;
}

void MclHashBucket_Clear(MclHashBucket *self, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashNode *node, *tmpNode;
	MCL_LINK_FOREACH_SAFE(&self->nodes, MclHashNode, link, node, tmpNode) {
		MclHashBucket_RemoveNode(self, node, allocator, valueDeleter);
	}
	self->count = 0;
}

uint32_t MclHashBucket_GetCount(const MclHashBucket *self) {
	return self ? self->count : 0;
}

bool MclHashBucket_IsEmpty(const MclHashBucket *self) {
	return MclHashBucket_GetCount(self) == 0;
}

MclStatus MclHashBucket_PushBack(MclHashBucket *self, MclHashKey key, MclHashValue value, MclHashNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(!MclHashBucket_Find(self, key));

	MclHashNode *node = MclHashNode_Create(key, value, allocator);
	MCL_ASSERT_VALID_PTR(node);

	MCL_LINK_INSERT_TAIL(&self->nodes, node, MclHashNode, link);
	return MCL_SUCCESS;
}

uint32_t MclHashBucket_RemoveBy(MclHashBucket *self, MclHashNodePred *pred, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	uint32_t removedCount = 0;
	MclHashNode *node, *tmpNode;
	MCL_LINK_FOREACH_SAFE(&self->nodes, MclHashNode, link, node, tmpNode) {
		if (MclHashNodePred_Predicate(pred, node)) {
			MclHashBucket_RemoveNode(self, node, allocator, valueDeleter);
			removedCount++;
		}
	}
	return removedCount;
}

typedef struct {
	MclHashNodePred pred;
	MclHashKey targetKey;
} TargetKeyPred;

MCL_PRIVATE bool TargetKeyPred_IsEqual(MclHashNodePred *pred, const MclHashNode *node) {
	TargetKeyPred *self = MCL_TYPE_REDUCT(pred, TargetKeyPred, pred);
	return self->targetKey == node->key;
}

uint32_t MclHashBucket_Remove(MclHashBucket *self, MclHashKey key, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	TargetKeyPred pred = {.pred = MCL_HASH_NODE_PRED(TargetKeyPred_IsEqual), .targetKey = key};
	return MclHashBucket_RemoveBy(self, &pred.pred, allocator, valueDeleter);
}

MclHashNode* MclHashBucket_Find(const MclHashBucket *self, MclHashKey key) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclHashNode *node;
	MCL_LINK_FOREACH(&self->nodes, MclHashNode, link, node) {
		if (node->key == key) {
			return node;
		}
	}
	return NULL;
}

MclStatus MclHashBucket_Accept(const MclHashBucket *self, MclHashNodeVisitor *visitor) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visitor);

	MclHashNode *node;
	MCL_LINK_FOREACH(&self->nodes, MclHashNode, link, node) {
		MclStatus ret = MclHashNodeVisitor_Visit(visitor, node);
		if (MCL_DONE(ret)) return MCL_SUCCESS;
		if (MCL_FAILED(ret)) return ret;
	}
	return MCL_SUCCESS;
}