#ifndef H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5
#define H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5

#include "mcl/list/list_node.h"
#include "mcl/list/list_node_allocator.h"

MCL_STDC_BEGIN

MCL_TYPE(MclList) {
    MclListNodeAllocator *allocator;
	MCL_LINK(MclListNode) nodes;
	size_t size;
};

MclList* MclList_CreateDefault();
MclList* MclList_Create(MclListNodeAllocator*);

void MclList_Delete(MclList*, MclListDataDestroy);

void MclList_Init(MclList*, MclListNodeAllocator*);
void MclList_Clear(MclList*, MclListDataDestroy);

///////////////////////////////////////////////////////////
MclStatus MclList_PushFrontNode(MclList*, MclListNode*);
MclStatus MclList_PushBackNode(MclList*, MclListNode*);

MclStatus MclList_InsertNodeBefore(MclList*, MclListNode* nextNode, MclListNode*);
MclStatus MclList_InsertNodeAfter(MclList*, MclListNode* prevNode, MclListNode*);

MclStatus MclList_RemoveNode(MclList*, MclListNode*, MclListDataDestroy);

MclListNode* MclList_FindNode(const MclList*, MclListData);

///////////////////////////////////////////////////////////
MclListNode* MclList_PushFront(MclList*, MclListData);
MclListNode* MclList_PushBack(MclList*, MclListData);

MclListNode* MclList_InsertBefore(MclList*, MclListNode* nextNode, MclListData);
MclListNode* MclList_InsertAfter(MclList*, MclListNode* prevNode, MclListData);

MclStatus MclList_RemoveData(MclList*, MclListData, MclListDataDestroy);

MclListData MclList_RemoveFirst(MclList*);
MclListData MclList_RemoveLast(MclList*);
MclListData MclList_RemoveByPred(MclList*, MclListDataPred, void*);

size_t MclList_RemoveAllByPred(MclList *, MclListDataPred, void*, MclListDataDestroy);

MclListData  MclList_FindByPred(const MclList*, MclListDataPred, void*);
void MclList_FindAllByPred(const MclList*, MclListDataPred, void*, MclList *result);

MclStatus MclList_Accept(const MclList*, MclListDataVisit, void*);

///////////////////////////////////////////////////////////////
MCL_INLINE size_t MclList_GetSize(const MclList *self) {
	return self ? self->size : 0;
}

MCL_INLINE bool MclList_IsEmpty(const MclList *self) {
	return MclList_GetSize(self) == 0;
}

MCL_INLINE MclListNode* MclList_GetFirst(MclList *self) {
	return MclList_IsEmpty(self) ? NULL : MCL_LINK_FIRST(&self->nodes);
}

MCL_INLINE MclListNode* MclList_GetLast(MclList *self) {
	return MclList_IsEmpty(self) ? NULL : MCL_LINK_LAST(&self->nodes);
}

MCL_INLINE MclListNode* MclList_GetHead(MclList *self) {
	return self ? (MclListNode*)(&self->nodes) : NULL;
}

MCL_INLINE MclListNode* MclList_GetNextOf(MclList *self, MclListNode *node) {
	return node ? ((MclListNode_GetNext(node) == MclList_GetHead(self)) ? NULL : MclListNode_GetNext(node)) : MclList_GetFirst(self);
}

MCL_INLINE MclListNode* MclList_GetPrevOf(MclList *self, MclListNode *node) {
	return node ? ((MclListNode_GetPrev(node) == MclList_GetHead(self)) ? NULL : MclListNode_GetPrev(node)) : MclList_GetFirst(self);
}

/////////////////////////////////////////////////////////////////
#define MCL_LIST_FOREACH(list, node)							\
	MCL_LINK_FOREACH((&(list)->nodes), MclListNode, link, node)

///////////////////////////////////////////////////////////////
#define MCL_LIST_FOREACH_SAFE(list, node, tmpNode)				\
	MCL_LINK_FOREACH_SAFE((&(list)->nodes), MclListNode, link, node, tmpNode)

/////////////////////////////////////////////////////////////////
#define MCL_LIST_FOREACH_CALL(list, type, visitor, ...)	    	\
	do {														\
		MclListNode *node = NULL;								\
		MCL_LIST_FOREACH((list), (node)) {						\
			(void)visitor((type*)((node)->data), ##__VA_ARGS__);\
		}														\
	} while(0)

/////////////////////////////////////////////////////////////////
#define MCL_LIST_FOREACH_CALL_ASSERT(list, type, visitor, ...)	\
	do {														\
		MclListNode *node = NULL;								\
		MCL_LIST_FOREACH((list), (node)) {						\
			MCL_ASSERT_SUCC_CALL(visitor((type*)((node)->data), ##__VA_ARGS__));\
		}														\
	} while(0)

/////////////////////////////////////////////////////////////////
#define MCL_LIST(LIST, ALLOCATOR)   							\
	{.allocator = (ALLOCATOR), .nodes = MCL_LINK_INITIALIZER(&(LIST).nodes, MclListNode, link), .size = 0}

#define MCL_LIST_DEFAULT(LIST) MCL_LIST(LIST, &MCL_LIST_NODE_ALLOCATOR_DEFAULT)

MCL_STDC_END

#endif
