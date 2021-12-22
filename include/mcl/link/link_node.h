#ifndef H22383929_F96E_447A_9D78_780E12E969AD
#define H22383929_F96E_447A_9D78_780E12E969AD

#include "mcl/link/link_data.h"
#include "mcl/stdtype.h"

MCL_STDC_BEGIN

MCL_TYPE_DEF(MclLinkNode) {
	MclLinkNode *next;
	MclLinkNode *prev;
	MclLinkData data;
};

MCL_INLINE void MclLinkNode_Init(MclLinkNode *self, MclLinkData data) {
    if (!self) return;
    self->next = NULL;
    self->prev = NULL;
    self->data = data;
}

MCL_INLINE MclLinkNode* MclLinkNode_GetNext(MclLinkNode *self) {
	return self ? self->next : NULL;
}

MCL_INLINE MclLinkNode* MclLinkNode_GetPrev(MclLinkNode *self) {
	return self ? self->prev : NULL;
}

MCL_INLINE MclLinkData  MclLinkNode_GetData(const MclLinkNode *self) {
	return self ? self->data : 0;
}

MCL_INLINE bool MclLinkNode_IsInLink(const MclLinkNode *self) {
	if (!self) return false;
	return (self->next) && (self->prev) && (self->next->prev == self) && (self->prev->next == self);
}

MCL_INLINE void MclLinkNode_RemoveFromLink(MclLinkNode *self) {
    if (!self || !MclLinkNode_IsInLink(self)) return;
    MclLinkNode *prevNode = self->prev;
    MclLinkNode *nextNode = self->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
}

MCL_TYPE_FWD(MclLinkNodeAllocator);

MclLinkNode* MclLinkNode_Create(MclLinkData, MclLinkNodeAllocator*);
void MclLinkNode_Delete(MclLinkNode*, MclLinkNodeAllocator*, MclLinkDataDeleter*);

#define MCL_LINK_NODE(DATA) {.next = NULL, .prev = NULL, .data = (DATA)}

MCL_STDC_END

#endif
