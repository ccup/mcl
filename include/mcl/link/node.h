#ifndef H22383929_F96E_447A_9D78_780E12E969AD
#define H22383929_F96E_447A_9D78_780E12E969AD

#include "mcl/keyword.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

MCL_STDC_BEGIN

typedef void* MclLinkData;
typedef void (*MclLinkDataDeleter)(MclLinkData);

MCL_TYPE_DEF(MclLinkNode) {
	MclLinkNode *next;
	MclLinkNode *prev;
	MclLinkData data;
};

MCL_INLINE MclLinkNode* MclLinkNode_GetNext(MclLinkNode *self) {
	return self ? self->next : NULL;
}

MCL_INLINE MclLinkNode* MclLinkNode_GetPrev(MclLinkNode *self) {
	return self ? self->prev : NULL;
}

MCL_INLINE MclLinkData  MclLinkNode_GetData(const MclLinkNode *self) {
	return self ? self->data : 0;
}

MCL_INLINE void MclLinkNode_SetData(MclLinkNode *self, MclLinkData data) {
	if (self) self->data = data;
}

MCL_INLINE bool MclLinkNode_IsDataEq(MclLinkNode *self, MclLinkData data) {
	return self->data == (MclLinkData)data;
}

MCL_INLINE void MclLinkNode_DeleteData(MclLinkNode *self, MclLinkDataDeleter deleter) {
	if (!self) return;
	if (!deleter || !self->data) return;
	deleter(self->data);
}

MCL_INLINE bool MclLinkNode_IsInLink(const MclLinkNode *self) {
	if (!self) return false;
	return (self->next) && (self->prev) && (self->next->prev == self) && (self->prev->next == self);
}

MclLinkNode* MclLinkNode_Create(MclLinkData);
void MclLinkNode_Delete(MclLinkNode*, MclLinkDataDeleter);
void MclLinkNode_RemoveFromLink(MclLinkNode*);

MCL_STDC_END

#endif
