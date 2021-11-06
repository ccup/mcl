#include "mcl/link.h"
#include "mcl/memory.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclLinkNode_Init(MclLinkNode *node) {
	(node)->next = NULL;
	(node)->prev = NULL;
}

MCL_PRIVATE bool MclLinkNode_IsInLink(MclLinkNode *node) {
	return (node->next) && (node->prev) && (node->next->prev == node) && (node->prev->next == node);
}

MCL_PRIVATE void MclLink_AddMiddle(MclLink *link, MclLinkNode *prevNode, MclLinkNode *node, MclLinkNode *nextNode) {
	link->count++;
	node->next = nextNode;
	node->prev = prevNode;
	prevNode->next = node;
	nextNode->prev = node;
}

MCL_PRIVATE void MclLink_AddBefore(MclLink *link, MclLinkNode *nextNode, MclLinkNode *node) {
	MclLink_AddMiddle(link, nextNode->prev, node, nextNode);
}

MCL_PRIVATE void MclLink_AddAfter(MclLink *link, MclLinkNode *prevNode, MclLinkNode *node) {
	MclLink_AddMiddle(link, prevNode, node, prevNode->next);
}

MCL_PRIVATE void MclLink_AddHead(MclLink *link, MclLinkNode *node) {
	MclLink_AddMiddle(link, &(link->head), node, link->head.next);
}

MCL_PRIVATE void MclLink_AddTail(MclLink *link, MclLinkNode *node) {
	MclLink_AddMiddle(link, link->head.prev, node, &(link->head));
}

MCL_PRIVATE void MclLink_Remove(MclLink *link, MclLinkNode *node) {
	MclLinkNode *prevNode = node->prev;
	MclLinkNode *nextNode = node->next;
	prevNode->next = nextNode;
	nextNode->prev = prevNode;
	MclLinkNode_Init(node);
	link->count--;
	MCL_FREE(node);
}

MCL_PRIVATE void MclLink_RemoveEx(MclLink *link, MclLinkNode *node) {
	MclLink_RemoveSafe(link, node);
	node->next = node;
	node->prev = node;
}

MclLink* MclLink_Create() {
	MclLink *link = (MclLink*)MCL_MALLOC(sizeof(MclLink));
	MCL_ASSERT_VALID_PTR_NIL(link);

	MckLink_Init(link);
	return link;
}

void MclLink_Delete(MclLink* link, MclLinkNodeHandleDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(link);

	MclLink_Clear(link, deleter);
	MCL_FREE(link);
}

void MckLink_Init(MclLink *link) {
	MCL_ASSERT_VALID_PTR_VOID(link);

	link->head.next = &(link->head);
	link->head.prev = &(link->head);
	link->count = 0;

}

void MclLink_Clear(MclLink *link, MclLinkNodeHandleDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(link);

	MclLinkNode *node = NULL;
	MclLinkNode *tmpNode = NULL;

	MCL_LINK_FOR_EACH_SAFE(link, node, tmpNode) {
		if (deleter && (node->handle != (uintptr_t)NULL)) {
			deleter(node->handle);
		}
		MclLink_Remove(link, node);
	}
}

bool MclLink_IsEmpty(const MclLink *link) {
	return link->count == 0;
}

uint32_t MclLink_GetSize(const MclLink *link) {
	MCL_ASSERT_VALID_PTR_NIL(link);
	return link->count;
}

void MclLink_AddBeforeSafe(MclLink *link, MclLinkNode *nextNode, MclLinkNode *node) {
	MCL_ASSERT_VALID_PTR_VOID(link);
	MCL_ASSERT_VALID_PTR_VOID(node);
	MCL_ASSERT_VALID_PTR_VOID(nextNode);

	MclLink_RemoveEx(link, node);
	MclLink_AddBefore(link, nextNode, node);
}

void MclLink_AddAfterSafe(MclLink *link, MclLinkNode *nextNode, MclLinkNode *node) {
	MCL_ASSERT_VALID_PTR_VOID(link);
	MCL_ASSERT_VALID_PTR_VOID(node);
	MCL_ASSERT_VALID_PTR_VOID(nextNode);

	MclLink_RemoveEx(link, node);
	MclLink_AddAfter(link, nextNode, node);
}

void MclLink_AddHeadSafe(MclLink *link, MclLinkNode *node) {
	MCL_ASSERT_VALID_PTR_VOID(link);
	MCL_ASSERT_VALID_PTR_VOID(node);

	MclLink_RemoveSafe(link, node);
	MclLink_AddHead(link, node);
}

void MclLink_AddTailSafe(MclLink *link, MclLinkNode *node) {
	MCL_ASSERT_VALID_PTR_VOID(link);
	MCL_ASSERT_VALID_PTR_VOID(node);

	MclLink_RemoveSafe(link, node);
	MclLink_AddTail(link, node);
}

void MclLink_RemoveSafe(MclLink *link, MclLinkNode *node) {
	MCL_ASSERT_VALID_PTR_VOID(link);
	MCL_ASSERT_VALID_PTR_VOID(node);

	if (MclLinkNode_IsInLink(node)) {
		MclLink_Remove(link, node);
	}
}

void MclLink_DeleteNode(MclLink *link, MclLinkNode *node, MclLinkNodeHandleDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(link);
	MCL_ASSERT_VALID_PTR_VOID(node);

	if (deleter) {
		deleter(node->handle);
	}
	MclLink_RemoveSafe(link ,node);
}

MclStatus MclLink_AddHeadWithHandle(MclLink *link, uintptr_t handle) {
	MCL_ASSERT_VALID_PTR(link);

	MclLinkNode *node = (MclLinkNode*)MCL_MALLOC(sizeof(MclLinkNode));
	MCL_ASSERT_VALID_PTR(node);

	if (!handle) {
		MCL_LOG_WARN("link add null handle.");
	}
	node->handle = handle;
	MclLink_AddHeadSafe(link ,node);

	return MCL_SUCCESS;
}

MclStatus MclLink_AddTailWithHandle(MclLink *link, uintptr_t handle) {
	MCL_ASSERT_VALID_PTR(link);

	MclLinkNode *node = (MclLinkNode*)MCL_MALLOC(sizeof(MclLinkNode));
	MCL_ASSERT_VALID_PTR(node);

	if (!handle) {
		MCL_LOG_WARN("link add null handle.");
	}
	node->handle = handle;
	MclLink_AddTailSafe(link ,node);

	return MCL_SUCCESS;
}

MclLinkNode* MclLink_FindNodeByHandle(const MclLink *link, uintptr_t handle) {
	MCL_ASSERT_VALID_PTR_NIL(link);

	MclLinkNode *node = NULL;
	MCL_LINK_FOR_EACH(link, node) {
		if (handle == node->handle) {
			return node;
		}
	}
	return NULL;
}

void MclLink_RemoveNodeWithHandle(MclLink *link, uintptr_t handle) {
	MCL_ASSERT_VALID_PTR_VOID(link);

	MclLinkNode *node = MclLink_FindNodeByHandle(link, handle);
	if (node) {
		MclLink_RemoveSafe(link, node);
	}

}

void MclLink_DeleteNodeWithHandle(MclLink *link, uintptr_t handle, MclLinkNodeHandleDeleter deleter) {
	MCL_ASSERT_VALID_PTR_VOID(link);

	if (deleter) {
		deleter(handle);
	}
	MclLink_RemoveNodeWithHandle(link, handle);
}

