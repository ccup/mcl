#ifndef MCL_9D4ABA5E943341339E3B717F62B7F8D1
#define MCL_9D4ABA5E943341339E3B717F62B7F8D1

#include "mcl/map/hash_bucket.h"
#include "mcl/map/hash_node_allocator.h"

MCL_STDC_BEGIN

MCL_TYPE(MclHashMap) {
	MclHashNodeAllocator *allocator;
    uint32_t elementCount;
    uint32_t bucketCount;
    MclHashBucket *buckets;
};

MclHashMap* MclHashMap_CreateDefault();
MclHashMap* MclHashMap_Create(uint32_t bucketCount, MclHashNodeAllocator*);

void MclHashMap_Delete(MclHashMap*, MclHashValueDestroy);

void MclHashMap_Init(MclHashMap*, uint32_t bucketCount, MclHashBucket*, MclHashNodeAllocator*);
void MclHashMap_Clear(MclHashMap*, MclHashValueDestroy);

///////////////////////////////////////////////////////////
MclStatus MclHashMap_InsertNode(MclHashMap*, MclHashNode*);
MclStatus MclHashMap_RemoveNode(MclHashMap*, MclHashNode*, MclHashValueDestroy);

MclHashNode* MclHashMap_FindNode(const MclHashMap*, MclHashKey);

///////////////////////////////////////////////////////////
MclStatus MclHashMap_Get(const MclHashMap*, MclHashKey, MclHashValue*);
MclHashValue MclHashMap_Set(MclHashMap*, MclHashKey, MclHashValue);

MclHashValue MclHashMap_FindByPred(const MclHashMap*, MclHashNodePred, void*);

MclHashValue MclHashMap_Remove(MclHashMap*, MclHashKey);
MclHashValue MclHashMap_RemoveByPred(MclHashMap*, MclHashNodePred, void*)
;
uint32_t MclHashMap_RemoveAllByPred(MclHashMap*, MclHashNodePred, void*, MclHashValueDestroy);

MclStatus MclHashMap_Accept(const MclHashMap*, MclHashNodeVisit, void*);

void MclHashMap_Dump(const MclHashMap*);

///////////////////////////////////////////////////////////
MCL_INLINE uint32_t MclHashMap_GetCount(const MclHashMap *self) {
    return self ? self->elementCount : 0;
}

MCL_INLINE bool MclHashMap_IsEmpty(const MclHashMap *self) {
    return MclHashMap_GetCount(self) == 0;
}

///////////////////////////////////////////////////////////
#define MCL_HASHMAP_BUCKET_COUNT_DEFAULT 127

#define MCL_HASHMAP(MAP, BUCKET_COUNT, BUCKETS, ALLOCATOR)   		\
	{.allocator = (ALLOCATOR), .elementCount = 0, .bucketCount = (BUCKET_COUNT), .buckets = (BUCKETS)}

#define MCL_HASHMAP_DEFAULT(MAP, BUCKET_COUNT, BUCKETS) 			\
	MCL_HASHMAP(MAP, BUCKET_COUNT, BUCKETS, &MCL_HASH_NODE_ALLOCATOR_DEFAULT)

MCL_STDC_END

#endif
