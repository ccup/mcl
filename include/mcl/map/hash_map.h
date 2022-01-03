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
void MclHashMap_Delete(MclHashMap*, MclHashValueDeleter*);

uint32_t MclHashMap_GetCount(const MclHashMap*);
bool MclHashMap_IsEmpty(const MclHashMap*);

void MclHashMap_Init(MclHashMap*, uint32_t bucketCount, MclHashBucket*, MclHashNodeAllocator*);
void MclHashMap_Clear(MclHashMap*, MclHashValueDeleter*);

MclHashNode* MclHashMap_FindNode(const MclHashMap*, MclHashKey);
MclStatus MclHashMap_InsertNode(MclHashMap*, MclHashNode*);
MclStatus MclHashMap_RemoveNode(MclHashMap*, MclHashNode*, MclHashValueDeleter*);

MclStatus MclHashMap_Get(const MclHashMap*, MclHashKey, MclHashValue*);
MclStatus MclHashMap_Set(MclHashMap*, MclHashKey, MclHashValue);

void MclHashMap_Remove(MclHashMap*, MclHashKey, MclHashValueDeleter*);
void MclHashMap_RemoveBy(MclHashMap*, MclHashNodePred*, MclHashValueDeleter*);

MclStatus MclHashMap_Accept(const MclHashMap*, MclHashNodeVisitor*);

void MclHashMap_Dump(const MclHashMap*);

///////////////////////////////////////////////////////////
#define MCL_HASHMAP_BUCKET_COUNT_DEFAULT 127

#define MCL_HASHMAP(MAP, BUCKET_COUNT, BUCKETS, ALLOCATOR)   		\
	{.allocator = (ALLOCATOR), .elementCount = 0, .bucketCount = (BUCKET_COUNT), .buckets = (BUCKETS)}

#define MCL_HASHMAP_DEFAULT(MAP, BUCKET_COUNT, BUCKETS) 			\
	MCL_HASHMAP(MAP, BUCKET_COUNT, BUCKETS, &MCL_HASH_NODE_ALLOCATOR_DEFAULT)

MCL_STDC_END

#endif
