#include "mcl/list/list.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclList_RemoveNodeFromList(MclList* self, MclListNode *node, MclListDataDestroy destroy) {
	MclListNode_RemoveFromList(node);
	MclListNode_Delete(node, self->allocator, destroy);
	self->size--;
}

MCL_PRIVATE void MclList_InsertBeforeNode(MclList *self, MclListNode *nextNode, MclListNode *node) {
	MCL_LINK_INSERT_BEFORE(nextNode, node, link);
	self->size++;
}

MCL_PRIVATE void MclList_InsertAfterNode(MclList *self, MclListNode *prevNode, MclListNode *node) {
	MCL_LINK_INSERT_AFTER(prevNode, node, link);
	self->size++;
}

MCL_PRIVATE void MclList_AddHead(MclList *self, MclListNode *node) {
	MCL_LINK_INSERT_HEAD(&self->nodes, node, MclListNode, link);
	self->size++;
}

MCL_PRIVATE void MclList_AddTail(MclList *self, MclListNode *node) {
	MCL_LINK_INSERT_TAIL(&self->nodes, node, MclListNode, link);
	self->size++;
}

MclList* MclList_CreateDefault() {
    return MclList_Create(&MclListNodeAllocator_Default);
}

MclList* MclList_Create(MclListNodeAllocator *allocator) {
	MclList *self = MCL_MALLOC(sizeof(MclList));
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclList_Init(self, allocator);
	return self;
}

void MclList_Delete(MclList* self, MclListDataDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclList_Clear(self, destroy);
	MCL_FREE(self);
}

void MclList_Init(MclList *self, MclListNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MCL_LINK_INIT(&self->nodes, MclListNode, link);
	self->size = 0;
	self->allocator = allocator;
}

void MclList_Clear(MclList *self, MclListDataDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclListNode *node = NULL;
	MclListNode *tmpNode = NULL;
	MCL_LIST_FOREACH_SAFE(self, node, tmpNode) {
		MclList_RemoveNodeFromList(self, node, destroy);
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

MclStatus MclList_RemoveNode(MclList *self, MclListNode *node, MclListDataDestroy destroy) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(node);
	MCL_ASSERT_TRUE_NIL(MclListNode_IsInList(node));

	MclList_RemoveNodeFromList(self, node, destroy);
	return MCL_SUCCESS;
}

MclListNode* MclList_FindNode(const MclList *self, MclListData data) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_TRUE_NIL(MclListData_IsValid(data));

    MclListNode *node = NULL;
    MCL_LIST_FOREACH((MclList*)self, node) {
        if (data == MclListNode_GetData(node)) return node;
    }
    return NULL;
}

MclListNode* MclList_PushFront(MclList *self, MclListData data) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclListData_IsValid(data));

	MclListNode *node = MclListNode_Create(data, self->allocator);
	MCL_ASSERT_VALID_PTR_NIL(node);

    MCL_ASSERT_SUCC_CALL_NIL(MclList_PushFrontNode(self, node));
    return node;
}

MclListNode*  MclList_PushBack(MclList *self, MclListData data) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclListData_IsValid(data));

	MclListNode *node = MclListNode_Create(data, self->allocator);
	MCL_ASSERT_VALID_PTR_NIL(node);

    MCL_ASSERT_SUCC_CALL_NIL(MclList_PushBackNode(self, node));
    return node;
}

MclListNode* MclList_InsertBefore(MclList *self, MclListNode *nextNode, MclListData data) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclListData_IsValid(data));

	MclListNode *node = MclListNode_Create(data, self->allocator);
	MCL_ASSERT_VALID_PTR_NIL(node);

    MCL_ASSERT_SUCC_CALL_NIL(MclList_InsertNodeBefore(self, nextNode, node));
    return node;
}

MclListNode* MclList_InsertAfter(MclList *self, MclListNode *prevNode, MclListData data) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclListData_IsValid(data));

	MclListNode *node = MclListNode_Create(data, self->allocator);
	MCL_ASSERT_VALID_PTR_NIL(node);

    MCL_ASSERT_SUCC_CALL_NIL(MclList_InsertNodeAfter(self, prevNode, node));
    return node;
}

MclStatus MclList_RemoveData(MclList *self, MclListData data, MclListDataDestroy destroy) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(MclListData_IsValid(data));

	MclListNode *node = MclList_FindNode(self, data);
	if (!node) return MCL_FAILURE;

	MclList_RemoveNodeFromList(self, node, destroy);
	return MCL_SUCCESS;
}

MclListData MclList_RemoveFirst(MclList *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclListNode *node = MclList_GetFirst(self);
	if (!node) return NULL;

	MclListData data = MclListNode_GetData(node);
	MclList_RemoveNodeFromList(self, node, NULL);
	return data;
}

MclListData MclList_RemoveLast(MclList *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclListNode *node = MclList_GetLast(self);
	if (!node) return NULL;

	MclListData data = MclListNode_GetData(node);
	MclList_RemoveNodeFromList(self, node, NULL);
	return data;
}

MclListData MclList_RemoveByPred(MclList *self, MclListDataPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

    MclListNode *node = NULL;
    MclListNode *tmpNode = NULL;
    MCL_LIST_FOREACH_SAFE((MclList*)self, node, tmpNode) {
        MclListData data = MclListNode_GetData(node);
        if (pred(data, arg)) {
            MclList_RemoveNodeFromList(self, node, NULL);
            return data;
        }
    }
    return NULL;
}

MclSize MclList_RemoveAllByPred(MclList *self, MclListDataPred pred, void *arg, MclListDataDestroy destroy) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	MclSize removedCount = 0;
    MclListNode *node = NULL;
    MclListNode *tmpNode = NULL;
    MCL_LIST_FOREACH_SAFE((MclList*)self, node, tmpNode) {
        if (pred(MclListNode_GetData(node), arg)) {
            MclList_RemoveNodeFromList(self, node, destroy);
            removedCount++;
        }
    }
    return removedCount;
}

MclListData  MclList_FindByPred(const MclList *self, MclListDataPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	MclListNode *node = NULL;
	MCL_LIST_FOREACH((MclList*)self, node) {
		MclListData data = MclListNode_GetData(node);
        if (pred(data, arg)) {
            return data;
        }
    }
	return NULL;
}

void MclList_FindAllByPred(const MclList *self, MclListDataPred pred, void *arg, MclList *result) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(pred);
	MCL_ASSERT_VALID_PTR_VOID(result);

	MclListNode *node = NULL;
	MCL_LIST_FOREACH((MclList*)self, node) {
		MclListData data = MclListNode_GetData(node);
        if (pred(data, arg)) {
            MclList_PushBack(result, data);
        }
    }
}

MclStatus MclList_Accept(const MclList *self, MclListDataVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclListNode *node = NULL;
	MCL_LIST_FOREACH((MclList*)self, node) {
		MclStatus ret = visit(MclListNode_GetData(node), arg);
		if (MCL_DONE(ret)) return MCL_SUCCESS;
		if (MCL_FAILED(ret)) return ret;
	}
	return MCL_SUCCESS;
}
