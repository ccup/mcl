#ifndef H22383929_F96E_447A_9D78_780E12E969AD
#define H22383929_F96E_447A_9D78_780E12E969AD

#include "mcl/list/list_data.h"
#include "mcl/link/link.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclListNodeAllocator);

MCL_TYPE(MclListNode) {
	MCL_LINK_NODE(MclListNode) link;
	MclListData data;
};

MCL_INLINE void MclListNode_Init(MclListNode *self, MclListData data) {
    if (!self) return;
    self->link.next = NULL;
    self->link.prev = NULL;
    self->data = data;
}

MCL_INLINE MclListNode* MclListNode_GetNext(MclListNode *self) {
	return self ? MCL_LINK_NODE_NEXT(self, link) : NULL;
}

MCL_INLINE MclListNode* MclListNode_GetPrev(MclListNode *self) {
	return self ? MCL_LINK_NODE_PREV(self, link) : NULL;
}

MCL_INLINE MclListData  MclListNode_GetData(const MclListNode *self) {
	return self ? self->data : 0;
}

MCL_INLINE bool MclListNode_IsInList(const MclListNode *self) {
	if (!self) return false;
	return MCL_LINK_NODE_IS_IN_LINK(self, link);
}

MCL_INLINE void MclListNode_RemoveFromList(MclListNode *self) {
    if (!self || !MclListNode_IsInList(self)) return;
    MCL_LINK_REMOVE(self, link);
}

MclListNode* MclListNode_Create(MclListData, MclListNodeAllocator*);
void MclListNode_Delete(MclListNode*, MclListNodeAllocator*, MclListDataDeleter*);

///////////////////////////////////////////////////////////////
#define MCL_LIST_NODE(DATA) {.link.next = NULL, .link.prev = NULL, .data = (DATA)}

MCL_STDC_END

#endif
