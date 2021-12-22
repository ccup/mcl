#ifndef MCL_9D4ABA5E943341339E3B717F62B7F8D1
#define MCL_9D4ABA5E943341339E3B717F62B7F8D1

#include "mcl/map/hash_bucket.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclHashNodeAllocator);

MCL_TYPE(MclHashMap) {
	MclHashNodeAllocator *allocator;
    uint32_t elementCount;
    uint32_t bucketCount;
    MclHashBucket buckets[];
};

#define MCL_HASHMAP_BUCKET_COUNT_DEFAULT (uint32_t)127

#define MCL_HASHMAP(MAP, BUCKET_COUNT, ALLOCATOR)   			\
{.allocator = ALLOCATOR, .elementCount = 0, .bucketCount = BUCKET_COUNT}

MclHashMap* MclHashMap_CreateDefault();
MclHashMap* MclHashMap_Create(uint32_t bucketCount, MclHashNodeAllocator*);
void MclHashMap_Delete(MclHashMap*, MclHashValueDeleter*);

uint32_t MclHashMap_GetCount(const MclHashMap*);
bool MclHashMap_IsEmpty(const MclHashMap*);

void MclHashMap_Init(MclHashMap*, uint32_t bucketCount, MclHashNodeAllocator*);
void MclHashMap_Clear(MclHashMap*, MclHashValueDeleter*);

MclHashNode* MclHashMap_FindNode(const MclHashMap*, MclHashKey);
MclStatus MclHashMap_InsertNode(MclHashMap*, MclHashNode*);
MclStatus MclHashMap_RemoveNode(MclHashMap*, MclHashNode*, MclHashValueDeleter*);

MclStatus MclHashMap_Get(const MclHashMap*, MclHashKey, MclHashValue*);
MclStatus MclHashMap_Set(MclHashMap*, MclHashKey, MclHashValue);

void MclHashMap_Remove(MclHashMap*, MclHashKey, MclHashValueDeleter*);
void MclHashMap_RemoveBy(MclHashMap*, MclHashNodePred*, MclHashValueDeleter*);

MclStatus MclHashMap_Accept(const MclHashMap*, MclHashNodeVisitor*);

MCL_STDC_END

#endif
