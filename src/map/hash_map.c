#include "mcl/map/hash_map.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE uint32_t MclHashMap_GetBucketId(const MclHashMap *self, MclHashKey key) {
    return key % self->bucketCount;
}

MclHashMap* MclHashMap_CreateDefault() {
    return MclHashMap_Create(MCL_HASHMAP_BUCKET_COUNT_DEFAULT, &MclHashNodeAllocator_Default);
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

    MclHashMap_Init(self, buckets, bucketCount, allocator);
    return self;
}

void MclHashMap_Delete(MclHashMap *self, MclHashValueDestroy destroy) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclHashMap_Clear(self, destroy);
    MCL_FREE(self->buckets);
    MCL_FREE(self);
}

void MclHashMap_Init(MclHashMap *self, MclHashBucket *buckets, uint32_t bucketCount, MclHashNodeAllocator *allocator) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(buckets);

    self->bucketCount = bucketCount;
    self->size = 0;
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
	self->size = 0;
}

MclStatus MclHashMap_InsertNode(MclHashMap *self, MclHashNode *node) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(node);

    uint32_t bucketId = MclHashMap_GetBucketId(self, node->key);
    MCL_ASSERT_SUCC_CALL(MclHashBucket_PushBackNode(&self->buckets[bucketId], node));
    self->size++;
    return MCL_SUCCESS;
}

MclStatus MclHashMap_RemoveNode(MclHashMap *self, MclHashNode *node, MclHashValueDestroy destroy) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(node);

    uint32_t bucketId = MclHashMap_GetBucketId(self, node->key);
    MCL_ASSERT_SUCC_CALL(MclHashBucket_RemoveNode(&self->buckets[bucketId], node, self->allocator, destroy));
    self->size--;
    return MCL_SUCCESS;
}

MclHashNode* MclHashMap_FindNode(const MclHashMap *self, MclHashKey key) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_TRUE_NIL(MclHashKey_IsValid(key));

	uint32_t bucketId = MclHashMap_GetBucketId(self, key);
	return MclHashBucket_FindNode(&self->buckets[bucketId], key);
}

MclHashValue MclHashMap_Get(const MclHashMap *self, MclHashKey key) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_TRUE_NIL(MclHashKey_IsValid(key));

    const MclHashNode *node = MclHashMap_FindNode(self, key);
    if (!node) return NULL;

    return MclHashNode_GetValue(node);
}

MclHashValue MclHashMap_Set(MclHashMap *self, MclHashKey key, MclHashValue value) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_TRUE_NIL(MclHashKey_IsValid(key));
    MCL_ASSERT_TRUE_NIL(MclHashValue_IsValid(value));

    MclHashNode *node = MclHashMap_FindNode(self, key);
    if (node) {
    	MclHashValue oriValue = MclHashNode_GetValue(node);
		node->value = value;
    	return oriValue;
    }

    node = MclHashNode_Create(key, value, self->allocator);
	MCL_ASSERT_VALID_PTR_NIL(node);

	if (MCL_FAILED(MclHashMap_InsertNode(self, node))) {
		MCL_LOG_ERR("Add node (%llu) to map failed when setting!", key);
		MclHashNode_Delete(node, self->allocator, NULL);
		return NULL;
	}
    return value;
}

MclHashValue MclHashMap_FindByPred(const MclHashMap *self, MclHashNodePred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
    	MclHashValue value = MclHashBucket_FindByPred(&self->buckets[i], pred, arg);
    	if (MclHashValue_IsValid(value)) {
    		return value;
    	}
    }
	return NULL;
}

MclHashValue MclHashMap_Remove(MclHashMap *self, MclHashKey key) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclHashKey_IsValid(key));

	uint32_t bucketId = MclHashMap_GetBucketId(self, key);

	MclHashValue value = MclHashBucket_Remove(&self->buckets[bucketId], key, self->allocator);
	if (MclHashValue_IsValid(value)) self->size--;
	return value;
}

MclHashValue MclHashMap_RemoveByPred(MclHashMap *self, MclHashNodePred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
    	MclHashValue value = MclHashBucket_RemoveByPred(&self->buckets[i], pred, arg, self->allocator);
    	if (MclHashValue_IsValid(value)) {
    		self->size--;
    		return value;
    	}
    }
	return NULL;
}

size_t MclHashMap_RemoveAllByPred(MclHashMap *self, MclHashNodePred pred, void *arg, MclHashValueDestroy destroy) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	size_t removedCount = 0;

    for (size_t i = 0; i < self->bucketCount; i++) {
    	removedCount += MclHashBucket_RemoveAllByPred(&self->buckets[i], pred, arg, self->allocator, destroy);
    }

	if (removedCount > self->size) {
		MCL_LOG_FATAL("Removed overflow (%u) from hash map which only has (%u) elements!", removedCount, self->size);
		self->size = 0;
	} else {
		self->size -= removedCount;
	}
	return removedCount;
}

MclStatus MclHashMap_Accept(const MclHashMap *self, MclHashNodeVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
        MclStatus ret = MclHashBucket_Accept(&self->buckets[i], visit, arg);
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
