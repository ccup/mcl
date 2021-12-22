#ifndef H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5
#define H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5

#include "mcl/link/link_node.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclLinkNodeAllocator);

typedef MclLinkNode MclLinkHead;

MCL_TYPE(MclLink) {
	MclLinkHead head;
	uint32_t count;
    MclLinkNodeAllocator *allocator;
};

#define MCL_LINK(LINK, ALLOCATOR)   \
{.head.next = &((LINK).head), .head.prev = &((LINK).head), .count = 0, .allocator = (ALLOCATOR)}

MclLink* MclLink_CreateDefault();
MclLink* MclLink_Create(MclLinkNodeAllocator*);
void MclLink_Delete(MclLink*, MclLinkDataDeleter*);

void MclLink_Init(MclLink*, MclLinkNodeAllocator*);
void MclLink_Clear(MclLink*, MclLinkDataDeleter*);

MclStatus MclLink_PushFrontNode(MclLink*, MclLinkNode*);
MclStatus MclLink_PushBackNode(MclLink*, MclLinkNode*);

MclStatus MclLink_InsertNodeBefore(MclLink*, MclLinkNode* nextNode, MclLinkNode*);
MclStatus MclLink_InsertNodeAfter(MclLink*, MclLinkNode* prevNode, MclLinkNode*);

MclStatus MclLink_PushFront(MclLink*, MclLinkData);
MclStatus MclLink_PushBack(MclLink*, MclLinkData);

MclStatus MclLink_InsertBefore(MclLink*, MclLinkNode* nextNode, MclLinkData);
MclStatus MclLink_InsertAfter(MclLink*, MclLinkNode* prevNode, MclLinkData);

void MclLink_RemoveNode(MclLink*, MclLinkNode*, MclLinkDataDeleter*);
void MclLink_RemoveData(MclLink*, MclLinkData, MclLinkDataDeleter*);

void MclLink_RemoveBy(MclLink *, MclLinkDataPred*, MclLinkDataDeleter *);

MclLinkNode* MclLink_FindNode(MclLink*, MclLinkData);
void MclLink_FindBy(const MclLink*, MclLinkDataPred*, MclLink *result);

MclStatus MclLink_Accept(const MclLink*, MclLinkDataVisitor*);

///////////////////////////////////////////////////////////////
MCL_INLINE uint32_t MclLink_GetCount(const MclLink *self) {
	return self ? self->count : 0;
}

MCL_INLINE bool MclLink_IsEmpty(const MclLink *self) {
	return MclLink_GetCount(self) == 0;
}

MCL_INLINE MclLinkNode* MclLink_GetFirst(MclLink *self) {
	return MclLink_IsEmpty(self) ? NULL : self->head.next;
}

MCL_INLINE MclLinkNode* MclLink_GetLast(MclLink *self) {
	return MclLink_IsEmpty(self) ? NULL : self->head.prev;
}

MCL_INLINE MclLinkNode* MclLink_GetNextOf(MclLink *self, MclLinkNode *node) {
	return node ? ((node->next == &self->head) ? NULL : node->next) : MclLink_GetFirst(self);
}

MCL_INLINE MclLinkNode* MclLink_GetPrevOf(MclLink *self, MclLinkNode *node) {
	return node ? ((node->prev == &self->head) ? NULL : node->prev) : MclLink_GetFirst(self);
}

///////////////////////////////////////////////////////////////
#define MCL_LINK_FOREACH(link, node)						\
	for ((node) = MclLink_GetFirst(link);					\
		(MclLinkNode*)NULL != (node);						\
		(node) = MclLink_GetNextOf((link), (node)))

///////////////////////////////////////////////////////////////
#define MCL_LINK_FOREACH_SAFE(link, node, tmpNode)			\
	for ((node) = MclLink_GetFirst(link), (tmpNode) = MclLink_GetNextOf((link), (node));\
		(MclLinkNode*)NULL != (node);						\
		(node) = (tmpNode), (tmpNode) = MclLink_GetNextOf((link), (node)))

///////////////////////////////////////////////////////////////
#define MCL_LINK_FOREACH_CALL(link, type, visitor, ...)	    \
	do {													\
		MclLinkNode *node = NULL;							\
		MCL_LINK_FOREACH((link), (node)) {					\
			(void)visitor((type*)((node)->data), ##__VA_ARGS__);\
		}													\
	} while(0)

///////////////////////////////////////////////////////////////
#define MCL_LINK_FOREACH_CALL_ASSERT(link, type, visitor, ...)\
	do {													\
		MclLinkNode *node = NULL;							\
		MCL_LINK_FOREACH((link), (node)) {					\
			MCL_ASSERT_SUCC_CALL(visitor((type*)((node)->data), ##__VA_ARGS__));\
		}													\
	} while(0)

MCL_STDC_END

#endif
