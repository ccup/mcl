#include "mcl/link/link.h"
#include "mcl/memory.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclLink_RemoveNodeFromLink(MclLink* self, MclLinkNode *node, MclLinkDataDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(node);

	MclLinkNode_RemoveFromLink(node);
	MclLinkNode_Delete(node, deleter);
	self->count--;
}

MCL_PRIVATE void MclLink_InsertNodeBetween(MclLink *self, MclLinkNode *prevNode, MclLinkNode *node, MclLinkNode *nextNode) {
	node->next = nextNode;
	node->prev = prevNode;
	prevNode->next = node;
	nextNode->prev = node;
	self->count++;
}

MCL_PRIVATE void MclLink_InsertBeforeNode(MclLink *self, MclLinkNode *nextNode, MclLinkNode *node) {
	MclLink_InsertNodeBetween(self, nextNode->prev, node, nextNode);
}

MCL_PRIVATE void MclLink_InsertAfterNode(MclLink *self, MclLinkNode *prevNode, MclLinkNode *node) {
	MclLink_InsertNodeBetween(self, prevNode, node, prevNode->next);
}

MCL_PRIVATE void MclLink_AddHead(MclLink *self, MclLinkNode *node) {
       MclLink_InsertNodeBetween(self, &(self->head), node, self->head.next);
}

MCL_PRIVATE void MclLink_AddTail(MclLink *self, MclLinkNode *node) {
       MclLink_InsertNodeBetween(self, self->head.prev, node, &(self->head));
}

MclLink* MclLink_Create() {
	MclLink *self = MCL_MALLOC(sizeof(MclLink));
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclLink_Init(self);
	return self;
}

void MclLink_Delete(MclLink* self, MclLinkDataDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLink_Clear(self, deleter);
	MCL_FREE(self);
}

void MclLink_Init(MclLink *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	self->head.next = &(self->head);
	self->head.prev = &(self->head);
	self->count = 0;
}

void MclLink_Clear(MclLink *self, MclLinkDataDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLinkNode *node = NULL;
	MclLinkNode *tmpNode = NULL;
	MCL_LINK_FOR_EACH_SAFE(self, node, tmpNode) {
		MclLink_RemoveNodeFromLink(self, node, deleter);
	}
}

MclStatus MclLink_PushFront(MclLink *self, MclLinkData data) {
	MCL_ASSERT_VALID_PTR(self);

	MclLinkNode *node = MclLinkNode_Create(data);
	MCL_ASSERT_VALID_PTR(node);

	MclLink_AddHead(self, node);
	return MCL_SUCCESS;
}

MclStatus MclLink_PushBack(MclLink *self, MclLinkData data) {
	MCL_ASSERT_VALID_PTR(self);

	MclLinkNode *node = MclLinkNode_Create(data);
	MCL_ASSERT_VALID_PTR(node);

	MclLink_AddTail(self ,node);
	return MCL_SUCCESS;
}

MclLinkNode* MclLink_FindNode(MclLink *self, MclLinkData data) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclLinkNode *node = NULL;
	MCL_LINK_FOR_EACH(self, node) {
		if (data == node->data) return node;
	}
	return NULL;
}

void MclLink_RemoveNode(MclLink *self, MclLinkNode *node, MclLinkDataDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(node);
	MCL_ASSERT_TRUE_VOID(MclLinkNode_IsInLink(node));

	MclLink_RemoveNodeFromLink(self, node, deleter);
}

void MclLink_RemoveNodeOfData(MclLink *self, MclLinkData data, MclLinkDataDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLinkNode *node = MclLink_FindNode(self, data);
	if (!node) return;

	MclLink_RemoveNodeFromLink(self, node, deleter);
}

MclStatus MclLink_InsertBefore(MclLink *self, MclLinkNode *nextNode, MclLinkData data) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(nextNode);

	MclLinkNode *node = MclLinkNode_Create(data);
	MCL_ASSERT_VALID_PTR(node);

	MclLink_InsertBeforeNode(self, nextNode, node);
	return MCL_SUCCESS;
}

MclStatus MclLink_InsertAfter(MclLink *self, MclLinkNode *prevNode, MclLinkData data) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(prevNode);

	MclLinkNode *node = MclLinkNode_Create(data);
	MCL_ASSERT_VALID_PTR(node);

	MclLink_InsertAfterNode(self, prevNode, node);
	return MCL_SUCCESS;
}

void MclLink_FindBy(const MclLink *self, MclLinkPred pred, void *arg, MclLink *result) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(pred);
	MCL_ASSERT_VALID_PTR_VOID(result);

	MclLinkNode *node = NULL;
	MCL_LINK_FOR_EACH((MclLink*)self, node) {
		if (pred(node->data, arg)) {
			MclLink_PushBack(result, node->data);
		}
	}
}

MclStatus MclLink_Accept(const MclLink* self, MclLinkVisiter visitor, void* arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visitor);

	MclLinkNode *node = NULL;
	MCL_LINK_FOR_EACH((MclLink*)self, node) {
		MclStatus ret = visitor(node->data, arg);
		if (MCL_DONE(ret)) return MCL_SUCCESS;
		if (MCL_FAILED(ret)) return ret;
	}
	return MCL_SUCCESS;
}
