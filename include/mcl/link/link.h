#ifndef H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5
#define H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5

#include "mcl/link/node.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

typedef MclLinkNode MclLinkHead;

MCL_TYPE_DEF(MclLink) {
	MclLinkHead head;
	uint32_t count;
};

MclLink* MclLink_Create();
void MclLink_Delete(MclLink*, MclLinkDataDeleter);

void MckLink_Init(MclLink*);
void MclLink_Clear(MclLink*, MclLinkDataDeleter);

MclStatus MclLink_PushFront(MclLink*, MclLinkData);
MclStatus MclLink_PushBack(MclLink*, MclLinkData);

MclStatus MclLink_InsertBefore(MclLink*, MclLinkNode* nextNode, MclLinkData);
MclStatus MclLink_InsertAfter(MclLink*, MclLinkNode* prevNode, MclLinkData);

void MclLink_RemoveNode(MclLink*, MclLinkNode*, MclLinkDataDeleter);
void MclLink_RemoveNodeOfData(MclLink*, MclLinkData, MclLinkDataDeleter);

MclLinkNode* MclLink_FindNode(MclLink*, MclLinkData);

typedef bool (*MclLinkPred)(MclLinkData, void *arg);
void MclLink_FindBy(const MclLink*, MclLinkPred, void *arg, MclLink *result);

typedef MclStatus (*MclLinkVisiter)(MclLinkData, void *arg);
MclStatus MclLink_Accept(const MclLink*, MclLinkVisiter, void *arg);

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
#define MCL_LINK_FOR_EACH(link, node)						\
	for ((node) = MclLink_GetFirst(link);					\
		(MclLinkNode*)NULL != (node);						\
		(node) = MclLink_GetNextOf((link), (node)))

///////////////////////////////////////////////////////////////
#define MCL_LINK_FOR_EACH_SAFE(link, node, tmpNode)			\
	for ((node) = MclLink_GetFirst(link), (tmpNode) = MclLink_GetNextOf((link), (node));\
		(MclLinkNode*)NULL != (node);						\
		(node) = (tmpNode), (tmpNode) = MclLink_GetNextOf((link), (node)))

///////////////////////////////////////////////////////////////
#define MCL_LINK_VISIT_CALL_VOID(link, type, visitor, ...)	\
	do {													\
		MclLinkNode *node = NULL;							\
		MCL_LINK_FOR_EACH((link), (node)) {					\
			(void)visitor((type*)((node)->data), ##__VA_ARGS__);\
		}													\
	} while(0)

///////////////////////////////////////////////////////////////
#define MCL_LINK_VISIT_CALL(link, type, visitor, ...)		\
	do {													\
		MclLinkNode *node = NULL;							\
		MCL_LINK_FOR_EACH((link), (node)) {					\
			MCL_ASSERT_SUCC_CALL(visitor((type*)((node)->data), ##__VA_ARGS__));\
		}													\
	} while(0)

MCL_STDC_END

#endif
