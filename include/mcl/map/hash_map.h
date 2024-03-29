#ifndef MCL_9D4ABA5E943341339E3B717F62B7F8D1
#define MCL_9D4ABA5E943341339E3B717F62B7F8D1

#include "mcl/map/hash_bucket.h"
#include "mcl/map/hash_node_allocator.h"

MCL_STDC_BEGIN

MCL_TYPE(MclHashMap) {
	MclHashNodeAllocator *allocator;
    MclHashBucket *buckets;
    MclSize bucketCount;
    MclSize size;
};

MclHashMap* MclHashMap_CreateDefault();
MclHashMap* MclHashMap_Create(MclSize bucketCount, MclHashNodeAllocator*);

void MclHashMap_Delete(MclHashMap*, MclHashValueDestroy);

/* if allocator is null, should only use node apis: insertNode removeNode and findNode */
void MclHashMap_Init(MclHashMap*, MclHashBucket*, MclSize bucketCount, MclHashNodeAllocator*);
void MclHashMap_Clear(MclHashMap*, MclHashValueDestroy);

///////////////////////////////////////////////////////////
MclStatus MclHashMap_InsertNode(MclHashMap*, MclHashNode*);
MclStatus MclHashMap_RemoveNode(MclHashMap*, MclHashNode*, MclHashValueDestroy);

MclHashNode* MclHashMap_FindNode(const MclHashMap*, MclHashKey);

///////////////////////////////////////////////////////////
MclHashValue MclHashMap_Get(const MclHashMap*, MclHashKey);
MclHashValue MclHashMap_Set(MclHashMap*, MclHashKey, MclHashValue);

MclHashValue MclHashMap_FindByPred(const MclHashMap*, MclHashNodePred, void*);

MclHashValue MclHashMap_Remove(MclHashMap*, MclHashKey);
MclHashValue MclHashMap_RemoveByPred(MclHashMap*, MclHashNodePred, void*);

MclSize MclHashMap_RemoveAllByPred(MclHashMap*, MclHashNodePred, void*, MclHashValueDestroy);

MclStatus MclHashMap_Accept(const MclHashMap*, MclHashNodeVisit, void*);

void MclHashMap_Dump(const MclHashMap*);

///////////////////////////////////////////////////////////
MCL_INLINE MclSize MclHashMap_GetSize(const MclHashMap *self) {
    return self ? self->size : 0;
}

MCL_INLINE bool MclHashMap_IsEmpty(const MclHashMap *self) {
    return MclHashMap_GetSize(self) == 0;
}

///////////////////////////////////////////////////////////
#define MCL_HASHMAP_BUCKET_COUNT_DEFAULT 127

#define MCL_HASHMAP(MAP, BUCKETS, BUCKET_COUNT, ALLOCATOR)   		\
	{.allocator = (ALLOCATOR), .buckets = (BUCKETS), .bucketCount = (BUCKET_COUNT), .size = 0}

#define MCL_HASHMAP_DEFAULT(MAP, BUCKETS, BUCKET_COUNT) 			\
	MCL_HASHMAP(MAP, BUCKETS, BUCKET_COUNT, &MclHashNodeAllocator_Default)

MCL_STDC_END

#endif
