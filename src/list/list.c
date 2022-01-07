#include "mcl/list/list.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclList_RemoveNodeFromList(MclList* self, MclListNode *node, MclListDataDeleter *dataDeleter) {
	MclListNode_RemoveFromList(node);
	MclListNode_Delete(node, self->allocator, dataDeleter);
	self->count--;
}

MCL_PRIVATE void MclList_InsertBeforeNode(MclList *self, MclListNode *nextNode, MclListNode *node) {
	MCL_LINK_INSERT_BEFORE(nextNode, node, link);
	self->count++;
}

MCL_PRIVATE void MclList_InsertAfterNode(MclList *self, MclListNode *prevNode, MclListNode *node) {
	MCL_LINK_INSERT_AFTER(prevNode, node, link);
	self->count++;
}

MCL_PRIVATE void MclList_AddHead(MclList *self, MclListNode *node) {
	MCL_LINK_INSERT_HEAD(&self->nodes, node, MclListNode, link);
	self->count++;
}

MCL_PRIVATE void MclList_AddTail(MclList *self, MclListNode *node) {
	MCL_LINK_INSERT_TAIL(&self->nodes, node, MclListNode, link);
	self->count++;
}

MclList* MclList_CreateDefault() {
    return MclList_Create(MclListNodeAllocator_GetDefault());
}

MclList* MclList_Create(MclListNodeAllocator *allocator) {
	MclList *self = MCL_MALLOC(sizeof(MclList));
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclList_Init(self, allocator);
	return self;
}

void MclList_Delete(MclList* self, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclList_Clear(self, dataDeleter);
	MCL_FREE(self);
}

void MclList_Init(MclList *self, MclListNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MCL_LINK_INIT(&self->nodes, MclListNode, link);
	self->count = 0;
	self->allocator = allocator;
}

void MclList_Clear(MclList *self, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclListNode *node = NULL;
	MclListNode *tmpNode = NULL;
	MCL_LIST_FOREACH_SAFE(self, node, tmpNode) {
		MclList_RemoveNodeFromList(self, node, dataDeleter);
	}
}

MclStatus MclList_PushFrontNode(MclList *self, MclListNode *node) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(node);

    MclList_AddHead(self, node);
    return MCL_SUCCESS;
}

MclStatus MclList_PushBackNode(MclList *self, MclListNode *node) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(node);

    MclList_AddTail(self ,node);
    return MCL_SUCCESS;
}

MclStatus MclList_InsertNodeBefore(MclList *self, MclListNode* nextNode, MclListNode *node) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(nextNode);
    MCL_ASSERT_VALID_PTR(node);

    MclList_InsertBeforeNode(self, nextNode, node);
    return MCL_SUCCESS;
}

MclStatus MclList_InsertNodeAfter(MclList *self, MclListNode* prevNode, MclListNode *node) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(prevNode);
    MCL_ASSERT_VALID_PTR(node);

    MclList_InsertAfterNode(self, prevNode, node);
    return MCL_SUCCESS;
}

MclStatus MclList_PushFront(MclList *self, MclListData data) {
    return MclList_PushFrontNode(self, MclListNode_Create(data, self->allocator));
}

MclStatus MclList_PushBack(MclList *self, MclListData data) {
    return MclList_PushBackNode(self, MclListNode_Create(data, self->allocator));
}

MclStatus MclList_InsertBefore(MclList *self, MclListNode *nextNode, MclListData data) {
    return MclList_InsertNodeBefore(self, nextNode, MclListNode_Create(data, self->allocator));
}

MclStatus MclList_InsertAfter(MclList *self, MclListNode *prevNode, MclListData data) {
    return MclList_InsertNodeAfter(self, prevNode, MclListNode_Create(data, self->allocator));
}

uint32_t MclList_RemoveNode(MclList *self, MclListNode *node, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(node);
	MCL_ASSERT_TRUE_NIL(MclListNode_IsInList(node));

	MclList_RemoveNodeFromList(self, node, dataDeleter);
	return 1;
}

uint32_t MclList_RemoveData(MclList *self, MclListData data, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclListNode *node = MclList_FindNode(self, data);
	if (!node) return 0;

	MclList_RemoveNodeFromList(self, node, dataDeleter);
	return 1;
}

uint32_t MclList_RemoveAll(MclList *self, MclListDataPred *pred, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	uint32_t removedCount = 0;
    MclListNode *node = NULL;
    MclListNode *tmpNode = NULL;
    MCL_LIST_FOREACH_SAFE((MclList*)self, node, tmpNode) {
        if (MclListDataPred_Predicate(pred, node->data)) {
            MclList_RemoveNodeFromList(self, node, dataDeleter);
            removedCount++;
        }
    }
    return removedCount;
}

MclListData MclList_RemoveFirst(MclList *self, MclListDataPred *pred, MclListDataDeleter *dataDeleter) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

    MclListNode *node = NULL;
    MclListNode *tmpNode = NULL;
    MCL_LIST_FOREACH_SAFE((MclList*)self, node, tmpNode) {
        if (MclListDataPred_Predicate(pred, node->data)) {
        	MclListData data = node->data;
            MclList_RemoveNodeFromList(self, node, dataDeleter);
            return dataDeleter ? NULL: data;
        }
    }
    return NULL;
}

MclListNode* MclList_FindNode(const MclList *self, MclListData data) {
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclListNode *node = NULL;
    MCL_LIST_FOREACH((MclList*)self, node) {
        if (data == node->data) return node;
    }
    return NULL;
}

MclListData  MclList_FindFirst(const MclList *self, MclListDataPred *pred) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	MclListNode *node = NULL;
	MCL_LIST_FOREACH((MclList*)self, node) {
        if (MclListDataPred_Predicate(pred, node->data)) {
            return node->data;
        }
    }
	return NULL;
}

void MclList_FindBy(const MclList *self, MclListDataPred *pred, MclList *result) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(pred);
	MCL_ASSERT_VALID_PTR_VOID(result);

	MclListNode *node = NULL;
	MCL_LIST_FOREACH((MclList*)self, node) {
        if (MclListDataPred_Predicate(pred, node->data)) {
            MclList_PushBack(result, node->data);
        }
    }
}

MclStatus MclList_Accept(const MclList *self, MclListDataVisitor *visitor) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visitor);

	MclListNode *node = NULL;
	MCL_LIST_FOREACH((MclList*)self, node) {
		MclStatus ret = MclListDataVisitor_Visit(visitor, node->data);
		if (MCL_DONE(ret)) return MCL_SUCCESS;
		if (MCL_FAILED(ret)) return ret;
	}
	return MCL_SUCCESS;
}
