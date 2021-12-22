#ifndef MCL_2A0884783E524E87B389A5EF72B3E856
#define MCL_2A0884783E524E87B389A5EF72B3E856

#include "mcl/map/hash_key.h"
#include "mcl/map/hash_value.h"
#include "mcl/link/link.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclHashNodeAllocator);

MCL_TYPE(MclHashNode) {
	MCL_LINK_NODE(MclHashNode) link;
    MclHashKey key;
    MclHashValue value;
};

#define MCL_NODE(KEY, VALUE) {.link.prev = NULL, .link.next = NULL, .key = KEY, .value = VALUE};

MclHashNode* MclHashNode_Create(MclHashKey, MclHashValue, MclHashNodeAllocator*);
void MclHashNode_Delete(MclHashNode*, MclHashNodeAllocator*, MclHashValueDeleter*);

MCL_INLINE void MclHashNode_Init(MclHashNode *self, MclHashKey key, MclHashValue value) {
	if (!self) return;
    self->link.next = NULL;
    self->link.prev = NULL;
    self->key = key;
    self->value = value;
}

//////////////////////////////////////////////////////////////
MCL_TYPE(MclHashNodePred) {
    bool (*pred)(MclHashNodePred*, const MclHashNode*);
};

MCL_INLINE bool MclHashNodePred_Predicate(MclHashNodePred *pred, const MclHashNode *node) {
    return (pred && pred->pred) ? pred->pred(pred, node) : false;
}

#define MCL_HASH_NODE_PRED(PRED) {.pred = PRED}

//////////////////////////////////////////////////////////////
MCL_TYPE(MclHashNodeVisitor) {
    MclStatus (*visit)(MclHashNodeVisitor*, MclHashNode*);
};

MCL_INLINE MclStatus MclHashNodeVisitor_Visit(MclHashNodeVisitor *visitor, MclHashNode *node) {
    return (visitor && visitor->visit) ? visitor->visit(visitor, node) : MCL_FAILURE;
}

#define MCL_HASH_NODE_VISITOR(VISITOR) {.visit = VISITOR}

MCL_STDC_END

#endif
