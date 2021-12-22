#ifndef H22383929_F96E_447A_9D78_780E12E969AD
#define H22383929_F96E_447A_9D78_780E12E969AD

#include "mcl/list/list_data.h"
#include "mcl/stdtype.h"

MCL_STDC_BEGIN

MCL_TYPE(MclListNode) {
	MclListNode *next;
	MclListNode *prev;
	MclListData data;
};

MCL_INLINE void MclListNode_Init(MclListNode *self, MclListData data) {
    if (!self) return;
    self->next = NULL;
    self->prev = NULL;
    self->data = data;
}

MCL_INLINE MclListNode* MclListNode_GetNext(MclListNode *self) {
	return self ? self->next : NULL;
}

MCL_INLINE MclListNode* MclListNode_GetPrev(MclListNode *self) {
	return self ? self->prev : NULL;
}

MCL_INLINE MclListData  MclListNode_GetData(const MclListNode *self) {
	return self ? self->data : 0;
}

MCL_INLINE bool MclListNode_IsInList(const MclListNode *self) {
	if (!self) return false;
	return (self->next) && (self->prev) && (self->next->prev == self) && (self->prev->next == self);
}

MCL_INLINE void MclListNode_RemoveFromList(MclListNode *self) {
    if (!self || !MclListNode_IsInList(self)) return;
    MclListNode *prevNode = self->prev;
    MclListNode *nextNode = self->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
}

MCL_TYPE_DECL(MclListNodeAllocator);

MclListNode* MclListNode_Create(MclListData, MclListNodeAllocator*);
void MclListNode_Delete(MclListNode*, MclListNodeAllocator*, MclListDataDeleter*);

#define MCL_LIST_NODE(DATA) {.next = NULL, .prev = NULL, .data = (DATA)}

MCL_STDC_END

#endif
