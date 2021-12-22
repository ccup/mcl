#include "mcl/map/hash_map.h"
#include "mcl/map/hash_node_allocator.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE uint32_t MclHashMap_GetBucketId(const MclHashMap *self, MclHashKey key)
{
    return key % self->bucketCount;
}

MclHashMap* MclHashMap_CreateDefault() {
    return MclHashMap_Create(MCL_HASHMAP_BUCKET_COUNT_DEFAULT, MclHashNodeAllocator_GetDefault());
}

MclHashMap* MclHashMap_Create(uint32_t bucketCount, MclHashNodeAllocator *allocator) {
    MCL_ASSERT_TRUE_NIL(bucketCount > 0);

    MclHashMap *self = MCL_MALLOC(sizeof(MclHashMap) + sizeof(MclHashBucket*) * bucketCount);
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclHashMap_Init(self, bucketCount, allocator);
    return self;
}

void MclHashMap_Delete(MclHashMap *self, MclHashValueDeleter *valueDeleter) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclHashMap_Clear(self, valueDeleter);
    MCL_FREE(self);
}

void MclHashMap_Init(MclHashMap *self, uint32_t bucketCount, MclHashNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    self->bucketCount = bucketCount;
    self->elementCount = 0;
    self->allocator = allocator;
    for (uint32_t i = 0; i < bucketCount; i++) {
        MclHashBucket_Init(&self->buckets[i]);
    }
}

void MclHashMap_Clear(MclHashMap *self, MclHashValueDeleter *valueDeleter) {
	for (uint32_t i = 0; i < self->bucketCount; i++) {
		MclHashBucket_Clear(&self->buckets[i], self->allocator, valueDeleter);
	}
	self->elementCount = 0;
}

uint32_t MclHashMap_GetCount(const MclHashMap *self) {
    return self ? self->elementCount : 0;
}

bool MclHashMap_IsEmpty(const MclHashMap *self) {
    return MclHashMap_GetCount(self) == 0;
}

MclStatus MclHashMap_Get(const MclHashMap *self, MclHashKey key, MclHashValue *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    uint32_t bucketId = MclHashMap_GetBucketId(self, key);

    MclHashNode *node = MclHashBucket_Find(&self->buckets[bucketId], key);
    if (!node) return MCL_FAILURE;

    (*value) = node->value;
    return MCL_SUCCESS;
}

MclStatus MclHashMap_Set(MclHashMap *self, MclHashKey key, MclHashValue value) {
    MCL_ASSERT_VALID_PTR(self);

    uint32_t bucketId = MclHashMap_GetBucketId(self, key);

    MclHashNode *node = MclHashBucket_Find(&self->buckets[bucketId], key);
    if (node) {
    	node->value = value;
    } else {
		MCL_ASSERT_SUCC_CALL(MclHashBucket_PushBack(&self->buckets[bucketId], key, value, self->allocator));
    }
    return MCL_SUCCESS;
}

void MclHashMap_Remove(MclHashMap *self, MclHashKey key, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	uint32_t bucketId = MclHashMap_GetBucketId(self, key);
	MclHashBucket_Remove(&self->buckets[bucketId], key, self->allocator, valueDeleter);
}

void MclHashMap_RemoveBy(MclHashMap *self, MclHashNodePred *pred, MclHashValueDeleter *valueDeleter) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(pred);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
        MclHashBucket_RemoveBy(&self->buckets[i], pred, self->allocator, valueDeleter);
    }
}

MclStatus MclHashMap_Accept(const MclHashMap *self, MclHashNodeVisitor *visitor) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visitor);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
        MclStatus ret = MclHashBucket_Accept(&self->buckets[i], visitor);
		if (MCL_DONE(ret)) return MCL_SUCCESS;
		if (MCL_FAILED(ret)) return ret;
    }
    return MCL_SUCCESS;
}
