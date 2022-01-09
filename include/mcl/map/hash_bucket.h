#ifndef H79E65FF7_EA86_4EEF_9167_75E1D61E959E
#define H79E65FF7_EA86_4EEF_9167_75E1D61E959E

#include "mcl/map/hash_node.h"

MCL_STDC_BEGIN

MCL_TYPE(MclHashBucket) {
	MCL_LINK(MclHashNode) nodes;
};

void MclHashBucket_Init(MclHashBucket*);
void MclHashBucket_Clear(MclHashBucket*, MclHashNodeAllocator*, MclHashValueDestroy);

MclStatus MclHashBucket_PushBackNode(MclHashBucket*, MclHashNode*);
MclStatus MclHashBucket_RemoveNode(MclHashBucket*, MclHashNode*, MclHashNodeAllocator*, MclHashValueDestroy);

MclHashNode* MclHashBucket_FindNode(const MclHashBucket*, MclHashKey);
MclHashValue MclHashBucket_FindByPred(const MclHashBucket*, MclHashNodePred, void*);

MclHashValue MclHashBucket_Remove(MclHashBucket*, MclHashKey, MclHashNodeAllocator*);
MclHashValue MclHashBucket_RemoveByPred(MclHashBucket*, MclHashNodePred, void*, MclHashNodeAllocator*);

uint32_t MclHashBucket_RemoveAllByPred(MclHashBucket*, MclHashNodePred, void*, MclHashNodeAllocator*, MclHashValueDestroy);

MclStatus MclHashBucket_Accept(const MclHashBucket*, MclHashNodeVisit, void*);

void MclHashBucket_Dump(const MclHashBucket*);

///////////////////////////////////////////////////////////
MCL_INLINE bool MclHashBucket_IsEmpty(const MclHashBucket *self) {
	return self ? MCL_LINK_EMPTY(&self->nodes, MclHashNode, link) : true;
}

///////////////////////////////////////////////////////////
#define MCL_HASH_BUCKET(BUCKET)   							\
{.nodes = MCL_LINK_INITIALIZER(&(BUCKET).nodes, MclHashNode, link)}

MCL_STDC_END

#endif
