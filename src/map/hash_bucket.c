#include "mcl/map/hash_bucket.h"
#include "mcl/assert.h"

void MclHashBucket_Init(MclHashBucket *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_LINK_INIT(&self->nodes, MclHashNode, link);
}

void MclHashBucket_Clear(MclHashBucket *self, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashNode *node, *tmpNode;
	MCL_LINK_FOREACH_SAFE(&self->nodes, MclHashNode, link, node, tmpNode) {
        (void)MclHashBucket_RemoveNode(self, node, allocator, valueDeleter);
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

MclStatus MclHashBucket_RemoveNode(MclHashBucket *self, MclHashNode *node, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_TRUE(MCL_LINK_NODE_IS_IN_LINK(node, link));
	MCL_LINK_REMOVE(node, link);
	MclHashNode_Delete(node, allocator, valueDeleter);
	return MCL_SUCCESS;
}

uint32_t MclHashBucket_RemoveBy(MclHashBucket *self, MclHashNodePred *pred, MclHashNodeAllocator *allocator, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	uint32_t removedCount = 0;
	MclHashNode *node, *tmpNode;
	MCL_LINK_FOREACH_SAFE(&self->nodes, MclHashNode, link, node, tmpNode) {
		if (MclHashNodePred_Predicate(pred, node)) {
			if (!MCL_FAILED(MclHashBucket_RemoveNode(self, node, allocator, valueDeleter))) {
			    removedCount++;
			}
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

void MclHashBucket_Dump(const MclHashBucket *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashNode *node;
	MCL_LINK_FOREACH(&self->nodes, MclHashNode, link, node) {
		MCL_LOG("Node (%llu : %p) -> ", node->key, node->value);
	}
	MCL_LOG("NULL\n");
}
