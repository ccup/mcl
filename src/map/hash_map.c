#include "mcl/map/hash_map.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE uint32_t MclHashMap_GetBucketId(const MclHashMap *self, MclHashKey key) {
    return key % self->bucketCount;
}

MclHashMap* MclHashMap_CreateDefault() {
    return MclHashMap_Create(MCL_HASHMAP_BUCKET_COUNT_DEFAULT, MclHashNodeAllocator_GetDefault());
}

MclHashMap* MclHashMap_Create(uint32_t bucketCount, MclHashNodeAllocator *allocator) {
    MCL_ASSERT_TRUE_NIL(bucketCount > 0);

    MclHashMap *self = MCL_MALLOC(sizeof(MclHashMap));
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclHashBucket *buckets = MCL_MALLOC(sizeof(MclHashBucket) * bucketCount);
    if (!buckets) {
        MCL_LOG_ERR("Malloc for buckets failed!");
        MCL_FREE(self);
        return NULL;
    }

    MclHashMap_Init(self, bucketCount, buckets, allocator);
    return self;
}

void MclHashMap_Delete(MclHashMap *self, MclHashValueDestroy destroy) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclHashMap_Clear(self, destroy);
    MCL_FREE(self);
}

void MclHashMap_Init(MclHashMap *self, uint32_t bucketCount, MclHashBucket *buckets, MclHashNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(buckets);

    self->bucketCount = bucketCount;
    self->elementCount = 0;
    self->allocator = allocator;
    self->buckets = buckets;
    for (uint32_t i = 0; i < bucketCount; i++) {
        MclHashBucket_Init(&self->buckets[i]);
    }
}

void MclHashMap_Clear(MclHashMap *self, MclHashValueDestroy destroy) {
	for (uint32_t i = 0; i < self->bucketCount; i++) {
		MclHashBucket_Clear(&self->buckets[i], self->allocator, destroy);
	}
	self->elementCount = 0;
}

uint32_t MclHashMap_GetCount(const MclHashMap *self) {
    return self ? self->elementCount : 0;
}

bool MclHashMap_IsEmpty(const MclHashMap *self) {
    return MclHashMap_GetCount(self) == 0;
}

MclHashNode* MclHashMap_FindNode(const MclHashMap *self, MclHashKey key) {
    MCL_ASSERT_VALID_PTR_NIL(self);

	uint32_t bucketId = MclHashMap_GetBucketId(self, key);
	return MclHashBucket_FindNode(&self->buckets[bucketId], key);
}

MclStatus MclHashMap_InsertNode(MclHashMap *self, MclHashNode *node) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(node);

    uint32_t bucketId = MclHashMap_GetBucketId(self, node->key);
    MCL_ASSERT_SUCC_CALL(MclHashBucket_PushBackNode(&self->buckets[bucketId], node));
    self->elementCount++;
    return MCL_SUCCESS;
}

MclStatus MclHashMap_RemoveNode(MclHashMap *self, MclHashNode *node, MclHashValueDestroy destroy) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(node);

    uint32_t bucketId = MclHashMap_GetBucketId(self, node->key);
    MCL_ASSERT_SUCC_CALL(MclHashBucket_RemoveNode(&self->buckets[bucketId], node, self->allocator, destroy));
    self->elementCount--;
    return MCL_SUCCESS;
}

MclStatus MclHashMap_Get(const MclHashMap *self, MclHashKey key, MclHashValue *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    const MclHashNode *node = MclHashMap_FindNode(self, key);
    if (!node) return MCL_FAILURE;

    (*value) = node->value;
    return MCL_SUCCESS;
}

MclStatus MclHashMap_Set(MclHashMap *self, MclHashKey key, MclHashValue value) {
    MCL_ASSERT_VALID_PTR(self);

    MclHashNode *node = MclHashMap_FindNode(self, key);
    if (node) {
		node->value = value;
    	return MCL_SUCCESS;
    }

    node = MclHashNode_Create(key, value, self->allocator);
	MCL_ASSERT_VALID_PTR(node);

	if (MCL_FAILED(MclHashMap_InsertNode(self, node))) {
		MCL_LOG_ERR("Add node (%llu) to map failed when setting!", key);
		MclHashNode_Delete(node, self->allocator, NULL);
		return MCL_FAILURE;
	}
    return MCL_SUCCESS;
}

void MclHashMap_Remove(MclHashMap *self, MclHashKey key, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	uint32_t bucketId = MclHashMap_GetBucketId(self, key);

	uint32_t removedCount = MclHashBucket_Remove(&self->buckets[bucketId], key, self->allocator, destroy);
	MCL_ASSERT_TRUE_VOID(removedCount <= self->elementCount);
	self->elementCount -= removedCount;
}

void MclHashMap_RemoveBy(MclHashMap *self, MclHashNodePred pred, void *arg, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(pred);

	uint32_t removedCount = 0;

    for (uint32_t i = 0; i < self->bucketCount; i++) {
    	removedCount += MclHashBucket_RemoveBy(&self->buckets[i], pred, arg, self->allocator, destroy);
    }
	MCL_ASSERT_TRUE_VOID(removedCount <= self->elementCount);
	self->elementCount -= removedCount;
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

void MclHashMap_Dump(const MclHashMap *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
    	if (!MclHashBucket_IsEmpty(&self->buckets[i])) {
    		MCL_LOG("HashMap dump : bucket %u \n", i);
    		MclHashBucket_Dump(&self->buckets[i]);
    	}
    }
}
