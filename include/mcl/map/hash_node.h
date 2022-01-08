#ifndef MCL_2A0884783E524E87B389A5EF72B3E856
#define MCL_2A0884783E524E87B389A5EF72B3E856

#include "mcl/map/hash_key.h"
#include "mcl/map/hash_value.h"
#include "mcl/link/link.h"
#include "mcl/keyword.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclHashNodeAllocator);

MCL_TYPE(MclHashNode) {
	MCL_LINK_NODE(MclHashNode) link;
    MclHashKey key;
    MclHashValue value;
};

MCL_INLINE void MclHashNode_Init(MclHashNode *self, MclHashKey key, MclHashValue value) {
	if (!self) return;
    self->link.next = NULL;
    self->link.prev = NULL;
    self->key = key;
    self->value = value;
}

MclHashNode* MclHashNode_Create(MclHashKey, MclHashValue, MclHashNodeAllocator*);
void MclHashNode_Delete(MclHashNode*, MclHashNodeAllocator*, MclHashValueDestroy);

MCL_INLINE MclHashKey MclHashNode_GetKey(const MclHashNode *self) {
	return self ? self->key : MCL_HASH_KEY_INVALID;
}

MCL_INLINE MclHashValue MclHashNode_GetValue(const MclHashNode *self) {
	return self ? self->value : NULL;
}

typedef bool (*MclHashNodePred)(const MclHashNode*, void*);
typedef MclStatus (*MclHashNodeVisit)(MclHashNode*, void*);

///////////////////////////////////////////////////////////
#define MCL_HASH_NODE(KEY, VALUE) {.link = MCL_LINK_NODE_INITIALIZER(), .key = (KEY), .value = (VALUE)}

MCL_STDC_END

#endif
