#include "mcl/link/node.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclLinkNode_Init(MclLinkNode *self, MclLinkData data) {
	self->next = NULL;
	self->prev = NULL;
	self->data = data;
}

MclLinkNode* MclLinkNode_Create(MclLinkData data) {
	MclLinkNode* self = MCL_MALLOC(sizeof(MclLinkNode));
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclLinkNode_Init(self, data);
	return self;
}

void MclLinkNode_Delete(MclLinkNode *self, MclLinkDataDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLinkNode_DeleteData(self, deleter);
	MCL_FREE(self);
}

void MclLinkNode_RemoveFromLink(MclLinkNode *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_TRUE_VOID(MclLinkNode_IsInLink(self));

	MclLinkNode *prevNode = self->prev;
	MclLinkNode *nextNode = self->next;
	prevNode->next = nextNode;
	nextNode->prev = prevNode;
}

