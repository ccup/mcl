#ifndef H79E65FF7_EA86_4EEF_9167_75E1D61E959E
#define H79E65FF7_EA86_4EEF_9167_75E1D61E959E

#include "mcl/map/hash_node.h"

MCL_STDC_BEGIN

MCL_TYPE(MclHashBucket) {
	MCL_LINK(MclHashNode) nodes;
};

void MclHashBucket_Init(MclHashBucket*);
void MclHashBucket_Clear(MclHashBucket*, MclHashNodeAllocator*, MclHashValueDeleter*);

bool MclHashBucket_IsEmpty(const MclHashBucket*);

MclHashNode* MclHashBucket_FindNode(const MclHashBucket*, MclHashKey);
MclStatus MclHashBucket_PushBackNode(MclHashBucket*, MclHashNode*);
MclStatus MclHashBucket_RemoveNode(MclHashBucket*, MclHashNode*, MclHashNodeAllocator*, MclHashValueDeleter*);

uint32_t MclHashBucket_Remove(MclHashBucket*, MclHashKey, MclHashNodeAllocator*, MclHashValueDeleter*);
uint32_t MclHashBucket_RemoveBy(MclHashBucket*, MclHashNodePred*, MclHashNodeAllocator*, MclHashValueDeleter*);

MclStatus MclHashBucket_Accept(const MclHashBucket*, MclHashNodeVisitor*);

void MclHashBucket_Dump(const MclHashBucket*);

///////////////////////////////////////////////////////////
#define MCL_HASH_BUCKET(BUCKET)   							\
{.nodes = MCL_LINK_INITIALIZER(&(BUCKET).nodes, MclHashNode, link)}

MCL_STDC_END

#endif
